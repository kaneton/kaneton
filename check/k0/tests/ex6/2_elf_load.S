.globl _start

.text
.code16

#define SKIP_KERNEL_PRELOAD

_start:
	push $0				/* This if for the 32bit ret */
	mov $32, %al
	call __kernel_preload
.code32
	mov %eax, %esi			/* Get kernel file pointer */
	call elf_load
1:
	jmp 1b

.code16

/* drive: %dl (floppy0: 0, floppy1: 1, drive0: 80h, drive1: 81h) */
__bios_disk_reset:
1:
	mov $0x0, %ah
	int $0x13
	jc 1b
	ret

/* drive: %dl, nbsectors: %al, track: %ch, sector: %cl, head: %dh */
/* buff: %bx */
__bios_read_sectors:
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
__pmode_switch:
	cli
	lgdt __gdtr
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
__gdtr:
	.short 24
	.int __gdt
__gdt:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
__gdt_cs:
	.byte 0xff, 0xff, 0x00, 0x00, 0x00, 0x9b, 0xdf, 0x00
__gdt_ds:
	.byte 0xff, 0xff, 0x00, 0x00, 0x00, 0x93, 0xdf, 0x00


/*
 * kernel_preload
 *
 * desc   : Preload a kernel executable on a floppy into memory and switch
 *	    to protected mode
 * input  : %al, number of sector to load. %dl, bios drive
 * output : %eax 
 */
__kernel_preload:
	call __bios_disk_reset
	
	mov %ds, %bx
	mov %bx, %es
	
	mov $0, %ch			/* track */
	mov $2, %cl			/* start sector */
	mov $0, %dx			/* head | drive: 0 */
	mov $0x7e00, %bx		/* Buffer addr  */
	call __bios_read_sectors
	push $0				/* This is for the 32bit ret */
	call __pmode_switch
.code32
	mov $0x7e00, %eax
	ret



#include "ex6/ex6.S"
