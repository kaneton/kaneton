.global bootloader_entry

bootloader_entry:
	push %ebx
	push %eax
	call bootloader
