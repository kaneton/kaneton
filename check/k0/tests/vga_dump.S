.text
.code16


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
	call __serial_dump
	add $0x02, %si
	inc %cx
	cmp $1920, %cx
	jl 1b
	popa
	ret
