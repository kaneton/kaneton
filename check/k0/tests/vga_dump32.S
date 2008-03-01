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

	cmp $'\n', %al		/* Ignore \n and \r on VGA frambuffer  */
	je 2f
	cmp $'\r', %al
	je 2f
	jmp 3f
2:
	mov $' ', %al
	
3:	call __serial32_dump
	add $0x02, %si
	inc %ecx
	cmp $1920, %ecx
	jl 1b

	mov $'\n', %al		/* Add \n (more clean) */
	call __serial32_dump 
	
	pop %esi
	pop %ecx
	ret

