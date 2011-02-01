.globl _start

.text
.code16

_start:
1:
	xor %ah, %ah
	int $0x13
	jnz 1b

	call floppy_init
	xor %ah, %ah
	int $0x13
	jnz 2f
	mov $'1', %al
	jmp 3f
2:
	mov $'0', %al
3:
	call __serial_dump
4:
	jmp 4b

#include "serial.S"

#include "ex4/ex4.S"
