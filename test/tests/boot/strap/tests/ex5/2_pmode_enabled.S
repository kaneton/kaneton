.globl _start

.text
.code16

_start:
	push $0		        /* This is for the 32bit ret */
	call pmode_switch
.code32
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	call __serial32_init
	movl __gdt_size, %eax
	sgdt (%eax)
	movw (%eax), %dx
	cmp $0x0018, %dx	/* Test GDT size */
	jne 2f

	movl 2(%eax), %eax

	movb 0x80, %bl
	
	movb 5(%eax), %dl	/* Test present bit for null descriptor */
	andb %bl, %dl
	jnz 2f
	
	movb 13(%eax), %dl	/* Test present bit for Code and Data
				   descriptors */
	andb %bl, %dl
	jz 2f
	
	movb 21(%eax), %dl
	andb %bl, %dl
	jz 2f
	
	mov $'H', %al
	call __serial32_dump
2:
	jmp 2b

__gdt_size:
	.short 0
__gdt_addr:
	.long 0

#include "serial32.S"
#include "vga_dump32.S"

#include "ex5/ex5.S"
