OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(_start)

SECTIONS
{
	. = 0x1000 + SIZEOF_HEADERS;

	.text :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata)
	}
}
