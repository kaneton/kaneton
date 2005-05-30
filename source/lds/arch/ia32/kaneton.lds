OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(kaneton)

SECTIONS
{
	. = 0x00200000 + SIZEOF_HEADERS;
	
	.text :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata)
	}	

	.data :
	{
		*(.data) *(.bss) *(COMMON)
	}
}