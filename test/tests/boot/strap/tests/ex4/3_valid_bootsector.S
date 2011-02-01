.globl _start

.text
.code16

_start:
	call is_bootsector
	call __vga_dump
1:
	jmp 1b
2:
	.ascii "LOL\0"

#include "print_string.S"
#include "serial.S"
#include "vga_dump.S"

#include "ex4/ex4.S"

	.org 1022, 0
	.short 0xaa55
