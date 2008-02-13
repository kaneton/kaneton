.globl _start

.text
.code16

_start:
	mov $0x0042, %ax
	call itoa_hex
	call __serial_strdump
1:
	jmp 1b

#include "serial.S"
#include "vga_dump.S"
#include "print_string.S"

#include "ex2/ex2.S"
