.text
.code16

.globl _start
_start:
	call __vga_clear
	mov $'A', %al
	call print_char
	call __vga_dump
1:
	jmp 1b

#include "serial.S"
#include "vga_dump.S"

#include "ex1/ex1.S"
