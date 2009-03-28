.global bootloader_entry
.global flag_address

	.data
	.align	8

flag_address:
	.quad	__bootloader_end

	.text
	.align	4

bootloader_entry:
	.set	noat
	.set	noreorder
	move $gp, $0

	# Initialize the stack pointer register
	lui $sp, 0xb000
	move $fp, $0
	move $ra, $0

	# Initialize the general pointer register

	lui $gp, 0xbfc0
	daddiu $gp, $gp, __offset

	# Clear all write right coprocessor 0 registers


	# Reset the TLB


	# Save the stack pointer in the fp register and in the stack
	daddiu $sp, $sp, -8
	move $fp, $sp
	sd $fp, 0($sp)
	jal bootloader
