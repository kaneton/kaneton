/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...ool/firmware/qemu-mips.mips64/firmware.c
 *
 * created       enguerrand raymond   [fri apr 10 13:11:23 2009]
 * updated       enguerrand raymond   [sun may 17 15:57:06 2009]
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
  firmware_cons_msg('+', "Kaneton firmware error\n");
  while (1)
    ;
}

static void		firmware_module_display(kaneton_mips_module*	kmmod,
						module_t*		mbi_mod,
						t_uint64		modn)
{
  firmware_cons_msg('+', "Module %i\n", modn);
  firmware_cons_msg('+', "kmmod = 0x%lx\n", kmmod);
  firmware_cons_msg('+', "kmmod->modsz = 0x%lx\n", kmmod->modsz);
  firmware_cons_msg('+', "kmmod->name = %s\n", kmmod->name);
  firmware_cons_msg('+', "kmmod->module = 0x%lx\n", kmmod->module);
  firmware_cons_msg('+', "mbi_mod->mod_start = 0x%lx\n", mbi_mod->mod_start);
  firmware_cons_print_string("\n");
}

/*
 * main firmware function (firmware entry)
 *
 * steps:
 *
 * 1) check the header image flag presence
 * 2) set the image_header just after the flag
 * 3) set main mbi fields and the destination pointer
 * 4) check the module number, if bootloader or kernel is absent
 *    the firmware goes to error state
 * 5) set the kaneton_mips_module in image header structure and the 
 *    mbi structure to set the mbi fields about the kaneton modules
 * 6) set the first mbi module_t(bootloader) structure about the first module and copy it
 *    to the destination
 * 7) set the rest of mbi module_t (kernel and kernel modules) structures about
 *    the all module loaded
 * 8) set the elf structure with bootloader elf to jump to the bootloader entry
 */

void			firmware(void)
{
  multiboot_info_t*	mbi = (multiboot_info_t*)MULTIBOOT_ADDRESS;
  kaneton_mips_header*	image_header = NULL;
  kaneton_mips_module*	kmmod = NULL;
  module_t*		mbi_mod = NULL;
  t_uint32		i = 0;
  t_vaddr		mod_dest = 0;
  Elf64_Ehdr*		kaneton_bootloader = NULL;
  void			(*bootloader)(multiboot_info_t*);

  /*
   * 1)
   */

  if(*flag_address != FIRMWARE_FLAG)
    firmware_error();

  firmware_cons_msg('+', "Kaneton qemu-mips firmware starting\n\n\n");

  /*
   * 2)
   */

  image_header = (kaneton_mips_header*)(flag_address + 1);

  /*
   * 3)
   */

  mbi->mods_count = image_header->nmod - 1;
  mbi->mods_addr = (unsigned long)mbi + sizeof(multiboot_info_t);
  mod_dest = IMAGE_BASE_ADDRESS;

  /*
   * 4)
   */

  if (mbi->mods_count < 1)
    firmware_error();

  /*
   * 5)
   */

  kmmod = (kaneton_mips_module*)(image_header + 1);
  mbi_mod = (module_t*)mbi->mods_addr;

  /*
   * 6)
   */

  firmware_cons_msg('+', "Kaneton image loading\n\n");

  mbi_mod->string = 0;
  mbi_mod->mod_start = mod_dest;
  kaneton_bootloader = (Elf64_Ehdr*)mod_dest;
  mbi_mod->mod_end = mbi_mod->mod_start + kmmod->modsz;

  firmware_module_display(kmmod, mbi_mod, 0);

  memcpy((t_uint8*)(mbi_mod->mod_start), (t_uint8*)(kmmod->module), kmmod->modsz);
  mod_dest += kmmod->modsz;

  kmmod += 1;
  mbi_mod += 1;

  /*
   * 7)
   */

  for (i = 0; i < mbi->mods_count; ++i)
    {
      mbi_mod->string = mod_dest;
      mod_dest += strlen(kmmod->name);
      
      mbi_mod->mod_start = mod_dest;
      mbi_mod->mod_end = mbi_mod->mod_start + kmmod->modsz;

      strcpy((t_uint8*)(mbi_mod->string), kmmod->name);
      memcpy((t_uint8*)(mbi_mod->mod_start), (t_uint8*)(kmmod->module), kmmod->modsz);
      
      firmware_module_display(kmmod, mbi_mod, i + 1);
      
      mod_dest += kmmod->modsz;
      kmmod += 1;
      mbi_mod += 1;
    }

  /*
   * 8)
   */

  bootloader = (void (*)(multiboot_info_t*))kaneton_bootloader->e_entry;
  
  firmware_cons_print_char('\n');
  firmware_cons_msg('+', "Jump to the bootloader function 0x%lx\n", bootloader);

  bootloader(mbi);
  
  firmware_error();
}
