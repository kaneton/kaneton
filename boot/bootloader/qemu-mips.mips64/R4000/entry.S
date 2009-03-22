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
	move $1, $0
	.set	at
	move $2, $0
	move $3, $0
	move $4, $0
	move $5, $0
	move $6, $0
	move $7, $0
	move $8, $0
	move $9, $0
	move $10, $0
  	move $11, $0
	move $12, $0
	move $13, $0
	move $14, $0
	move $15, $0
	move $16, $0
	move $17, $0
	move $18, $0
	move $19, $0
	move $20, $0
	move $21, $0
	move $22, $0
	move $23, $0
	move $24, $0
	move $25, $0
	move $26, $0
	move $27, $0
	move $gp, $0

	# Initialize the stack pointer register
	lui $sp, 0xb000
	move $fp, $0
	move $ra, $0

	# Initialize the general pointer register

	la $gp, __gp

	# Clear all write right coprocessor 0 registers


	# Reset the TLB


	# Save the stack pointer in the fp register and in the stack
	daddiu $sp, $sp, -8
	move $fp, $sp
	sd $fp, 0($sp)
	jal bootloader
