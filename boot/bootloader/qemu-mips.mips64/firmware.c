/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...t/bootloader/qemu-mips.mips64/firmware.c
 *
 * created       enguerrand raymond   [fri apr 10 13:11:23 2009]
 * updated       enguerrand raymond   [wed apr 22 08:25:52 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "firmware.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

extern t_uint32*	flag_address;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * a funny function which does nothing.
 *
 * this function is called when a fatal error occurs.
 */

void			firmware_error(void)
{
  while (1)
    ;
}

void			firmware(void)
{
  multiboot_info_t*	mbi = (multiboot_info_t*)MULTIBOOT_ADDRESS;
  kaneton_mips_header*	image_header = NULL;
  kaneton_mips_module*	kmmod = NULL;
  module_t*		mbi_mod = NULL;
  t_uint32		i = 0;
  t_vaddr		mod_dest = 0;

  /*
   * 1)
   */

  if(*flag_address != FIRMWARE_FLAG)
    firmware_error();

  /*
   * 2)
   */

 
  /*
   * 3)
   */

  image_header = (kaneton_mips_header*)(flag_address + 1);
  
  /*
   * 4)
   */

  mbi->mods_count = image_header->nmod - 1;
  mbi->mods_addr = (unsigned long)mbi + sizeof(multiboot_info_t);
  mod_dest = IMAGE_BASE_ADDRESS;

  /*
   * 5)
   */

  if (mbi->mods_count < 2)
    firmware_error();

  /*
   * 6)
   */

  kmmod = (kaneton_mips_module*)(image_header + 1);
  mbi_mod = (module_t*)mbi->mods_addr;

  /*
   * 7)
   */

  mbi_mod->string = 0;
  mbi_mod->mod_start = mod_dest;
  mbi_mod->mod_end = mbi_mod->mod_start + kmmod->modsz;
  
  memcpy((char*)(mbi_mod->mod_start), (char*)(kmmod->module), kmmod->modsz);

  kmmod += 1;
  mbi_mod += 1;

  /*
   * 8)
   */

  for (i = 0; i < mbi->mods_count; ++i)
    {
      mbi_mod->string = mod_dest;
      mod_dest += strlen(kmmod->name);

      mbi_mod->mod_start = mod_dest;
      mbi_mod->mod_end = mbi_mod->mod_start + kmmod->modsz;

      strcpy((char*)(mbi_mod->string), kmmod->name);
      memcpy((char*)(mbi_mod->mod_start), (char*)(kmmod->module), kmmod->modsz);

      kmmod += 1;
      mbi_mod += 1;
    }

  while(1);
}
