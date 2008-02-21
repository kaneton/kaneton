.globl _start

.text
.code16

_start:
	mov $42, %ax
	call itoa_hex
	call __serial_strdump
1:
	jmp 1b

#include "serial.S"

#include "ex2/ex2.S"
