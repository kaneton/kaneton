.globl _start

.text
.code16

_start:
	mov $0xDEAD, %ax
	mov $0x0000, %bx
	mov $0x0001, %cx
	mov $0x1000, %dx
	mov $0xffff, %si
	mov $0xf98c, %di
	call dump_registers
	call __vga_dump
1:
	jmp 1b

#include "serial.S"
#include "vga_dump.S"

#include "ex2/ex2.S"
