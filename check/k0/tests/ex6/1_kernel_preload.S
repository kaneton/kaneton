.globl _start

.text
.code16

#define SKIP_ELF_LOAD

_start:
	push $0				/* This if for the 32bit ret */
	mov $32, %al
	call kernel_preload
.code32
	mov %eax, %esi			/* Get kernel file pointer */
	call __elf_load

__memcpy:
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

__memset:
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
__elf_load:
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

	call   __memcpy


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

	call   __memset

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


#include "ex6/ex6.S"
