.globl _start

.text
.code16

_start:
	push $0		        /* This is for the 32bit ret */
	call pmode_switch
.code32
	call __serial32_init
	movb $'Z', %al
	call __serial32_dump
1:
	jmp 1b

#include "serial32.S"

#include "ex5/ex5.S"
