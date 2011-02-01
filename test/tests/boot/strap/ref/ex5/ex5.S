.code16


/*
 * pmode_switch
 *
 * desc   : Switch from real mode to protected mode.
 * input  : nothing
 * output : nothing
 */
pmode_switch:
	cli
	lgdt gdtr
	push %eax
	mov %cr0, %eax
	or $1, %eax
	mov %eax, %cr0
	pop %eax
	ljmp $0x8, $1f
.code32
1:
	mov $0x0010, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ret

.code16
gdtr:
	.short 24
	.int gdt
gdt:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
gdt_cs:
	.byte 0xff, 0xff, 0x00, 0x00, 0x00, 0x9b, 0xdf, 0x00
gdt_ds:
	.byte 0xff, 0xff, 0x00, 0x00, 0x00, 0x93, 0xdf, 0x00


.code32
/*
 * print_string32
 *
 * desc   : Print a string in 32-bit mode.
 * input  : %ecx, row
 *          %edx, column
 *          %esi, string
 * output : nothing
 */
print_string32:
	pusha
	mov $0xb8000, %edi
	
	mov $160, %eax
	push %edx
	mul %ecx
	add %eax, %edi
	pop %edx
	add %edx, %edi
	add %edx, %edi
1:
	lodsb
	test %al, %al
	jz 2f
	stosb
	inc %edi
	jmp 1b
2:
	popa
 	ret


/*
 * signature
 */
signature:
	.org 510, 0
	.short 0xaa55
