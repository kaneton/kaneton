	.text
	.align	4
	.set noreorder
	.set noat

xtlb_refill_handler:
	jal xtlb_refill
	nop

	.rept 0x1e
	nop
	.endr

cache_error_handler:
	jal cache_error
	nop

	.rept 0x1e
	nop
	.endr

general_exception_handler:
	jal general_exception
	nop

