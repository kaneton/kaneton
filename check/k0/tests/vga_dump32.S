.text
.code32

/*
 * __vga32_dump
 *
 * desc   : Dump the ascii content of the vga text framebuffer
	    to the serial port.
 * input  : nothing
 * output : nothing
 */
__vga32_dump:
	push %ecx
	push %esi

	call __serial32_init
	movl $0x0, %ecx
	movl $0xB8000, %esi
1:
	movb (%esi), %al
	call __serial32_dump
	add $0x02, %si
	inc %ecx
	cmp $1920, %ecx
	jl 1b
	
	pop %esi
	pop %ecx
	ret

