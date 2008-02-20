.code16

/* drive: %dl (floppy0: 0, floppy1: 1, drive0: 80h, drive1: 81h) */
bios_disk_reset:
1:
	mov $0x0, %ah
	int $0x13
	jc 1b
	ret

/* drive: %dl, nbsectors: %al, track: %ch, sector: %cl, head: %dh */
/* buff: %bx */
bios_read_sectors:
1:
	mov $0x2, %ah
	int $0x13
	jc 1b
	ret

/*
 * pmode_switch
 *
 * desc   : Switch from real mode to protected mode.
 * input  : nothing
 * output : nothing
 */
pmode_switch:
	cli
	lgdt gdtr
	push %eax
	mov %cr0, %eax
	or $1, %eax
	mov %eax, %cr0
	pop %eax
	ljmp $0x8, $1f
.code32
1:
	mov $0x0010, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ret

.code16
gdtr:
	.short 24
	.int gdt
gdt:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
gdt_cs:
	.byte 0xff, 0xff, 0x00, 0x00, 0x00, 0x9b, 0xdf, 0x00
gdt_ds:
	.byte 0xff, 0xff, 0x00, 0x00, 0x00, 0x93, 0xdf, 0x00


/*
 * kernel_preload
 *
 * desc   : Preload a kernel executable on a floppy into memory and switch
 *	    to protected mode
 * input  : %al, number of sector to load. %dl, bios drive
 * output : %eax 
 */
kernel_preload:
	call bios_disk_reset
	
	mov %ds, %bx
	mov %bx, %es
	
	mov $0, %ch			/* track */
	mov $2, %cl			/* start sector */
	mov $0, %dx			/* head | drive: 0 */
	mov $0x7e00, %bx		/* Buffer addr  */
	call bios_read_sectors
	push $0				/* This is for the 32bit ret */
	call pmode_switch
.code32
	mov $0x7e00, %eax
	ret

memcpy:
	push   %esi
	push   %edi
	push   %ecx

	cld

	movl   16(%esp), %edi		/* dst */
	movl   20(%esp), %esi		/* src */
	movl   24(%esp), %ecx		/* sz */

	jmp   2f
1:	
	lodsb
	stosb
	
	dec    %ecx
2:
	test   %ecx, %ecx
	jnz    1b
	
	pop    %ecx
	pop    %edi
	pop    %esi
	ret

memset:
	push   %edi
	push   %ecx

	cld

	movl   12(%esp), %edi		/* dst */
	movl   16(%esp), %eax		/* c */
	movl   20(%esp), %ecx		/* sz */

	rep stosb

	pop    %ecx
	pop    %edi
	ret


/*
 * elf_load
 *
 * desc   : Load and execute an ELF file.
 * input  : %esi, Pointer to the ELF file
 * output : nothing
 */
elf_load:
	push   %edi
	push   %ebx

	/*
	 * 1) Check the ELF header magic number
	 */

	cmpl   $0x464c457f,(%esi)	/* Check ELF magic number */
	je     2f
1:
	jmp    1b
2:

	/*
	 * 2) Get the entry point address
	 */

	mov    0x18(%esi),%eax		/* Get entry point */
	push   %eax			/* Save entry point */


	/*
	 * 3) Iterate on the PHDR table in the ELF header to relocalize segments.
	 */

	mov    %esi,%ebx		
	add    0x1c(%esi),%ebx		/* Get PHDR table address */
	
	xor    %edi,%edi		/* Loop throug PHDR table */
	jmp    4f
	
3:
	/*
	 * 5) Memory copy each segments at its relocalization address
	 *
	 * memcpy(Segment virtual address,
	 *        ELF file pointer + Segment file offset,
	 *        Segment 'in-file' size)
	 */
	pushl  -0x10(%ebx)		/* Get segment size in file */

	mov    %esi,%eax
	add    -0x1c(%ebx),%eax		/* Get segment address in file */
	push   %eax

	pushl  -0x18(%ebx)		/* Get segment reloc address */

	call   memcpy


	/*
	 * 6) Clear 'not loaded' segment parts (like bss section for example)
	 *
	 * NOTE : Each segments has an 'in-file' size and an 'in-memory' size,
	 *   This means that a segment can have a part existing in memory which does
	 *   not exists in the file. These parts should be cleared with zeroes.
	 *   This is the case for example when the segment contain a bss section.
	 *   A bss section appear into the memory image of the programm, but does not
	 *   appear into the file, because we already know it will contain only zeroes.
	 *
	 *   The segment parts which has to be cleared is computed through making the
	 *   difference between 'in-memory' segment size, and 'in-file' segment size.
	 */

	mov    -0x10(%ebx),%edx		/* Segment size in file */
	mov    -0xc(%ebx),%eax		/* Segment size in memory */
	sub    %edx,%eax		/* Size to reset */
	push   %eax

	push   $0x0

	add    -0x18(%ebx),%edx		/* address of the part to clear */
	push   %edx

	call   memset

	add    $0x18,%esp
4:
	/*
	 * 7) Loop epilog
	 */

	movzwl 0x2c(%esi),%eax		/* get the Program header table entry count */
	add    $0x20,%ebx		/* increment the Program header table iterator */
	inc    %edi
	cmp    %eax,%edi		/* finished ? */
	jl     3b

	/*
	 * 8) Jump to the entry point
	 */

	pop    %eax			/* restore entry point */
	jmp    *%eax			/* jump on entry point */
5:
	jmp    5b

/*
 * signature
 */
signature:
	.org 510, 0
	.short 0xaa55

