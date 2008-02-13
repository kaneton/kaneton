.globl _start

.text
.code16

_start:
	call floppy_init
	xor %ah, %ah
	int $0x13
	jnz 1f
	mov $'1', %al
	jmp 2f
1:
	mov $'0', %al
2:
	call __serial_dump
3:
	jmp 3b

#include "serial.S"

#include "ex4/ex4.S"
