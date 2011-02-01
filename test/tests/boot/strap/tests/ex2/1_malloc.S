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
	mov %di, %dx

	sub %cx, %dx /* Compute allocated memory space */
	sub %bx, %cx

	call __serial_init
	mov %cl, %al
	call __serial_dump
	mov %ch, %al
	call __serial_dump
	mov %dl, %al
	call __serial_dump
	mov %dh, %al
	call __serial_dump
1:
	jmp 1b

#include "serial.S"

#include "ex2/ex2.S"
