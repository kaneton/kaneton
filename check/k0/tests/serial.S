.text
.code16


/*
 * __serial_init
 *
 * desc   : initializes the serial port
 * input  : nothing
 * output : nothing
 */
__serial_init:
	pusha
	mov $227, %ax
	xor %dx, %dx
	int $0x14
	popa
	ret


/*
 * __serial_dump
 *
 * desc   : dump bytes to serial port
 * input  : %al, byte to dump
 * output : nothing
 */
__serial_dump:
	pusha
	xor %dx, %dx
	mov $1, %ah
	int $0x14
	popa
	ret


/*
 * __serial_strdump
 *
 * desc   : dump a string to serial port
 * input  : %si, pointer to string dump
 * output : nothing
 */
__serial_strdump:
	pusha
	xor %dx, %dx
	mov $1, %ah
	cld
1:
	mov $1, %ah
	lodsb
	test %al, %al
	jz 2f
	int $0x14
	jmp 1b
2:
	popa
	ret
