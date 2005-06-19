/*
 * kaneton
 *
 * multiboot.h
 *
 * path          /home/mycure/kaneton/core/include/arch/ia32/ia32
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 18:49:33 2004   mycure
 * last update   Sun Jun 19 14:09:34 2005   mycure
 */

#ifndef IA32_IA32_MULTIBOOT_H
#define IA32_IA32_MULTIBOOT_H	1

/*
 * defines
 */

#define MULTIBOOT_BOOTLOADER_MAGIC	0x2BADB002

/*
 * structures / types
 */

typedef struct			multiboot_header
{
  unsigned int			magic;
  unsigned int			flags;
  unsigned int			checksum;
  unsigned int			header_addr;
  unsigned int			load_addr;
  unsigned int			load_end_addr;
  unsigned int			bss_end_addr;
  unsigned int			entry_addr;
}				multiboot_header_t;

typedef struct			aout_symbol_table
{
  unsigned int			tabsize;
  unsigned int			strsize;
  unsigned int			addr;
  unsigned int			reserved;
}				aout_symbol_table_t;

typedef struct			elf_section_header_table
{
  unsigned int			num;
  unsigned int			size;
  unsigned int			addr;
  unsigned int			shndx;
}				elf_section_header_table_t;

typedef struct			multiboot_info
{
  unsigned int			flags;
  unsigned int			mem_lower;
  unsigned int			mem_upper;
  unsigned int			boot_device;
  unsigned int			cmdline;
  unsigned int			mods_count;
  unsigned int			mods_addr;

  union
  {
    aout_symbol_table_t		aout_sym;
    elf_section_header_table_t	elf_sec;
  }				u;

  unsigned int			mmap_length;
  unsigned int			mmap_addr;
}				multiboot_info_t;

typedef struct			module
{
  unsigned int			mod_start;
  unsigned int			mod_end;
  unsigned int			string;
  unsigned int			reserved;
}				module_t;

typedef struct			memory_map
{
  unsigned int			size;
  unsigned int			base_addr_low;
  unsigned int			base_addr_high;
  unsigned int			length_low;
  unsigned int			length_high;
  unsigned int			type;
}				memory_map_t;

#endif
