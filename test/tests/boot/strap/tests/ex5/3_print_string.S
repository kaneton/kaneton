.globl _start

.text
.code16

_start:
	push $0				/* This is for the 32bit ret */
	call pmode_switch
.code32
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	mov $3, %ecx
	mov $12, %edx
	mov $__str, %esi

	call print_string32

	call __vga32_dump
1:
	jmp 1b
	
__str:
.ascii	"Chiche, donne nous tout !\0"


#include "serial32.S"
#include "vga_dump32.S"

#include "ex5/ex5.S"
