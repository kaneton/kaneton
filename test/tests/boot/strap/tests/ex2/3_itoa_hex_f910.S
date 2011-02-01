.globl _start

.text
.code16

_start:
	mov $0xf910, %ax
	call itoa_hex
	call __serial_strdump
1:
	jmp 1b

#include "serial.S"

#include "ex2/ex2.S"
