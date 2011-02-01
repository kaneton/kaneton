.globl _start

.text
.code16

_start:
	mov $0x0001, %dx
	mov $2f, %si
	call print_string
	call __vga_dump
1:
	jmp 1b
2:
	.ascii "\0"

#include "serial.S"
#include "vga_dump.S"

#include "ex1/ex1.S"
