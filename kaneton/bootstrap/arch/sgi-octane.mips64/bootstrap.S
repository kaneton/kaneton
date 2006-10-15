	.text
	.extern _start
_start:
	mfc0 $12, $10
	andi $10, $10, 0xff7f
	;; mtc0 $10, $12
	j _entry
	nop
	.rept 0x200 / 8 - 1
	;; j exception_handler
	nop
	.endr



	.ent _entry
_entry:
	# zero all
	# don't use the temp reg, since it's the first
	.set noat
	move $1, $0
	.set at
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
	move $28, $0
	move $29, $0
	move $30, $0

	# coprocessor registers
	mtc0 $0, $4
	mtc0 $0, $8
	mtc0 $0, $14

	# clear tlb: zero all (64) entries
	li $10, 64
	li $11, 0xfc0
tlb_reset:
	addiu $10, $10, -1
	sll $9, $10, 8
	mtc0 $9, $0
	mtc0 $0, $2
	mtc0 $11, $10
	tlbwi
	bnez $10, tlb_reset
	nop
	mtc0 $0, $10

init_stack:
	xor $sp, $sp, $sp
	la $sp, __kvaddr_sp
	move $fp, $sp
	la $gp, _gp

kloop:
	jal octaneload
	j kloop
	nop
	.end _entry
