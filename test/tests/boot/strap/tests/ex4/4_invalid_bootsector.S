.globl _start

.text
.code16

_start:
	call is_bootsector
	call __vga_dump
1:
	jmp 1b


#include "serial.S"
#include "vga_dump.S"

#include "ex4/ex4.S"


	.org 1022, 0
	.short 0xa242
