.code16


/*
 * floppy_init
 *
 * desc   : Initialize floppy drive.
 * input  : nothing
 * output : nothing
 */
floppy_init:
	push %ax
	mov $0x00, %dl // drive number
1:
	xor %ah, %ah
	int $0x13
	jnz 1b
	pop %ax
	ret


/*
 * is_bootsector
 * desc   : Load the first sector of the floppy and
	    check whether is a valid bootsector.
 * input  : nothing
 * input  : nothing
 */
is_bootsector:
	call floppy_init
	pusha
	mov $0x02, %ah // interrupt number
	mov $0x01, %al // number of sectors to read
	mov $0x00, %ch // cylinder number
	mov $0x02, %cl // sector number
	mov $0x00, %dh // head number
	mov $0x00, %dl // drive number
	xor %bx, %bx
	push %ax
	mov $0x2000, %ax
	mov %ax, %es
	pop %ax
	int $0x13

	// get the signature in %ax
	add $510, %bx
	mov %es:(%bx), %ax
	cmp $0xaa55, %ax
	jnz 1f
	// it is a valid bootsector
	mov $2f, %si
	call print_string
	popa
	ret
1:
	// it is not a valid bootsector
	mov $3f, %si
	call print_string
	call itoa_hex
	call print_string
	popa
	ret

2:
	.ascii "magic found: 0xaa55\0"
3:
	.ascii "wrong magic: \0"


/*
 * print_string
 *
 * desc   : Print a string using BIOS services.
 * input  : %si, string
 * output : nothing
 */
print_string:
	pusha
	cld
1:
	lodsb
	test %al, %al
	jz 2f
	mov $0x0e, %ah
	xor %dh, %dh
	mov %bl, %bl
	int $0x10
	jmp 1b
2:
	popa
	ret

/*
 * itoa_hex
 *
 * desc   : Convert an integer into string using hexadecimal representation.
 * input  : %ax, number to convert
 * output : %si, pointer to the string
 */
itoa_hex:
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

	ret


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
 * signature
 */
signature:
	.org 510, 0
	.short 0xaa55
