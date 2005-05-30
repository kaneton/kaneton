OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(bootloader_entry)

SECTIONS
{
	. = 0x100000 + SIZEOF_HEADERS;
	
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