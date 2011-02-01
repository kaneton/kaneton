.code16


/*
 * get_key
 *
 * desc   : Get noth keycode and ASCII code from the keyboard buffer.
 * input  : nothing
 * output : %al, ASCII code from the keyboard buffer
	    %ah, keyboard scan code
 */
get_key:
	xor %ah, %ah
	int $0x16
	ret


/*
 * getln
 *
 * desc   : Print typed text on a screen. Press <enter> results in a newline.
 * input  : nothing
 * output : nothing
 */
getln:
	push %ax
1:
	call get_key
	cmp $0x0d, %al
	jz 2f
	mov %al, %ah
	call print_char
	jmp 1b
2:
	call line_return
	jmp 1b

	pop %ax
	ret


/*
 * print_char
 *
 * desc   : Print a character on the screen using BIOS services.
 * input  : %ah, ascii code of the character to be printed
 * output : nothing
 */
print_char:
	pusha
	mov %ah, %al
	mov $0x0E, %ah
	xor %dh, %dh
	xor %bl, %bl
	int $0x10
	popa
	ret


/*
 * line_return
 *
 * desc   : Line return with vertical rotation.
 * input  : nothing
 * output : nothing
 */
line_return:
	pusha
	// get cursor position
	mov $0x03, %ah
	xor %bh, %bh
	int $0x10
	// next row
	inc %dh
	cmp $25, %dh
	jnz 1f
	xor %dh, %dh
1:
	// column 0
	xor %dl, %dl
	// set cursor position
	mov $0x02, %ah
	mov $0, %bh
	int $0x10
	popa
	ret


/*
 * signature
 */
signature:
	.org 510, 0
	.short 0xaa55
