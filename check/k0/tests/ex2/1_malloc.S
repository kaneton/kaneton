.globl _start

.text
.code16

_start:
	mov $42, %ax
	call malloc
	mov %di, %bx
	mov $21, %ax
	call malloc
	mov %di, %cx
	call malloc

	call __serial_init
	mov %bl, %al
	call __serial_dump
	mov %bh, %al
	call __serial_dump
	mov %cl, %al
	call __serial_dump
	mov %ch, %al
	call __serial_dump
	mov %di, %ax
	call __serial_dump
	mov %ah, %al
	call __serial_dump

1:
	jmp 1b

#include "serial.S"

#include "ex2/ex2.S"
