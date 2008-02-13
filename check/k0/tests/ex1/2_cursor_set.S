.globl _start

.text
.code16

_start:
	mov $0x1010, %dx
	call cursor_set
	mov $2f, %si
	call __print_string
	call __vga_dump
1:
	jmp 1b
2:
	.ascii "Hello world!\0"


#include "serial.S"
#include "vga_dump.S"
#include "print_string.S"

#include "ex1/ex1.S"
