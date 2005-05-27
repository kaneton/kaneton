/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.c
 * 
 * path          /home/mycure/data/research/projects/kaneton/source/core/bootloader/arch/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri May 27 12:19:41 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

#define CONS_ADDR 0xb8000
#define CONS_SIZE 0x0fa0
#define CONS_ATTR_DEFAULT 0x0f

void			bootloader_error(void)
{
  char*			message = "[!] bootloader error detected";
  char*			cons = (char*)CONS_ADDR;
  int			i;

  for (i = 0; i < CONS_SIZE; i++)
    {
      cons[i * 2] = ' ';
      cons[(i * 2) + 1] = CONS_ATTR_DEFAULT;
    }

  for (i = 0; message[i]; i++)
    {
      cons[i * 2] = message[i];
      cons[(i * 2) + 1] = CONS_ATTR_DEFAULT;
    }

  while (1)
    ;
}

int			main(t_uint32		magic,
			     multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		header;

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  //gdt_init();

  /*
      pmap_init(minfo);
      vmap_init(minfo);
      gdt_reinit();

      header = (Elf32_Ehdr *)mod->mod_start;
      kernel = (t_func)header->e_entry;

      asm ("mov %0, %%esp" : : "g" (sp_init));

      kernel(VMAP_MNG_ADDR, minfo);
    }
  */

  while (1)
    ;
}
