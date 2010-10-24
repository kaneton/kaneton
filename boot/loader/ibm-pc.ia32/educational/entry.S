.global loader_entry

loader_entry:
	push %ebx
	push %eax
	call bootloader
