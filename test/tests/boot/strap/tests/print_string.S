.code16


/*
 * __print_string
 *
 * desc   : Print a string using BIOS services.
 * input  : %dh, row
            %dl, column
	    %si, string
 * output : nothing
 */
__print_string:
	pusha
	mov $0x02, %ah
	xor %bh, %bh
	int $0x10
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
