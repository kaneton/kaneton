.globl _start

.text
.code16

_start:
	push $0				/* This if for the 32bit ret */
	mov $32, %al
	call kernel_preload
.code32
	mov %eax, %esi			/* Get kernel file pointer */
	call elf_load

#include "ex6/ex6.S"
