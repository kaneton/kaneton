.code16


/*
 * malloc
 *
 * desc   : Allocate memory in a very simple way.
 * input  : %ax, size to allocate
 * output : %di, pointer to the memory area
 */
malloc:
	mov (break), %di
	add %ax, break
	ret
break:
	.short 0x9000


/*
 * itoa_hex
 *
 * desc   : Convert an integer into string using hexadecimal representation.
 * input  : %ax, number to convert
 * output : %si, pointer to the string
 */
itoa_hex:
        push %ax
	push %cx
	push %bx
	push %di
	push %dx

	push %ax
	xor %cx, %cx	// cx = 0
	mov $16, %bx	// dl = 10
1:
	mov $4, %cx
	mov $7, %ax
	call malloc	// malloc(strlen + 1)
	mov %ax, %di	// di = ax
	mov %di, %si	// si = di

	add %cx, %di
	add $2, %di
	movb $0, (%di)
	dec %di

	pop %ax
	mov $16, %bx
2:
	xor %dx, %dx	// dx = 0
	div %bx		// ax = dx:ax / bx, dx = dx:ax mod dx

	cmp $10, %dl
	jl 3f
	add $87, %dl
	jmp 4f

3:
	add $'0', %dl
4:
	mov %dl, (%di)
	dec %di
	dec %cx
	test %cx, %cx
	jnz 2b

	movb $'x', (%di)
	movb $'0', -1(%di)

        pop %dx
	pop %di
	pop %bx
	pop %cx
	pop %ax
	ret

/*
 * dump_registers
 *
 * desc   : Dump some register values.
 * input  : nothing
 * output : nothing
 */
dump_registers:
	pusha

	push %si

	mov $1f, %si
	call print_string
	call itoa_hex
	call print_string
	call line_return

	mov $2f, %si
	call print_string
	mov %bx, %ax
	call itoa_hex
	call print_string
	call line_return

	mov $3f, %si
	call print_string
	mov %cx, %ax
	call itoa_hex
	call print_string
	call line_return

	mov $4f, %si
	call print_string
	mov %dx, %ax
	call itoa_hex
	call print_string
	call line_return

	mov $5f, %si
	call print_string
	pop %si
	mov %si, %ax
	call itoa_hex
	call print_string
	call line_return

	mov $6f, %si
	call print_string
	mov %di, %ax
	call itoa_hex
	call print_string
	call line_return

	popa
	ret
1:
	.ascii "ax = \0"
2:
	.ascii "bx = \0"
3:
	.ascii "cx = \0"
4:
	.ascii "dx = \0"
5:
	.ascii "si = \0"
6:
	.ascii "di = \0"

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
 * print_string
 *
 * desc   : Print a string using BIOS services.
 * input  : %dh, row
            %dl, column
	    %si, string
 * output : nothing
 */
print_string:
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
