.code16


/*
 * print_char
 *
 * desc   : Print a character on the screen using BIOS services.
 * input  : %al, ascii code of the character to be printed
 * output : nothing
 */
print_char:
	pusha
	mov $0x0e, %ah
	xor %dh, %dh
	mov %bl, %bl
	int $0x10
	popa
	ret


/*
 * cursor_set
 *
 * desc   : Set cursor position.
 * input  : %dh, row
            %dl, column
 * output : nothing
 */
cursor_set:
	pusha
	mov $0x02, %ah
	xor %bh, %bh
	int $0x10
	popa
	ret


/*
 * print_string
 *
 * desc   : Print a string using BIOS services.
 * input  : %dh, row
            %dl, column
	    %si, string
 * output : nothing
 */
print_string:
	call cursor_set
	push %ax
	cld
1:
	lodsb
	test %al, %al
	jz 2f
	call print_char
	jmp 1b
2:
	pop %ax
	ret


/*
 * signature
 */
signature:
	.org 510, 0
	.short 0xaa55
