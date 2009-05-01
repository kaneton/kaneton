	.section        .multiboot,"aw",@progbits
	.align		4
	
#define	MAGIC		0x1BADB002
	.long		MAGIC

#define FLAGS		0x7		
			/* PAGE_ALIGN | MEMORY_INFO | VIDEO_INFO */
	.long		FLAGS
	.long		-(MAGIC + FLAGS)
