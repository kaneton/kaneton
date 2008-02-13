.globl _start

.text
.code16

_start:
	call dump_registers
	call __vga_dump
1:
	jmp 1b

#include "serial.S"
#include "vga_dump.S"

#include "ex2/ex2.S"
