.text
.code16

/*
 * __vga_clear
 *
 * desc   : Clear VGA framebuffer.
 * input  : nothing
 * output : nothing
 */
__vga_clear:
	pusha
	call __serial_init
	mov $0, %cx
	mov $0xb000, %ax
	mov %ax, %fs
	mov $0x8000, %si
1:
	movb $' ', %fs:(%si)
	add $0x02, %si
	inc %cx
	cmp $1920, %cx
	jl 1b
	popa
	ret


/*
 * __vga_dump
 *
 * desc   : Dump the ascii content of the vga text framebuffer
	    to the serial port.
 * input  : nothing
 * output : nothing
 */
__vga_dump:
	pusha
	call __serial_init
	mov $0, %cx
	mov $0xb000, %ax
	mov %ax, %fs
	mov $0x8000, %si
1:
	mov %fs:(%si), %al
	
	cmp $'\n', %al		/* Ignore \n and \r on VGA frambuffer  */
	jz 2f
	cmp $'\r', %al
	jz 2f
	jmp 3f

2:	mov $' ', %al

3:	call __serial_dump

	add $0x02, %si
	inc %cx
	cmp $1920, %cx
	jl 1b
	mov $'\n', %al		/* Add \n to diff files (more clean) */
	call __serial_dump
	popa
	ret
