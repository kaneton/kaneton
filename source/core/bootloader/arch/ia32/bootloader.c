/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.c
 * 
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon May 30 22:50:05 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

t_kaneton		kaneton;

void			(*kernel)(t_kaneton*);
t_reg32			ebp;
t_reg32			esp;

/*
 * a funny function which do nothing.
 */

void			bootloader_error(void)
{
  while (1)
    ;
}

/*
 * this function relocates the modules over the two megabytes,
 * creates a kernel stack etc.. and returns the kernel entry point address.
 *
 * this is not necessary but we want to it properly.
 *
 * note that the first module will be considered as the kernel module.
 *
 * 1) copies the kernel code into its new location KANETON_KERNEL_CODE
 * 2) initializes the kernel stack located at KANETON_KERNEL_STACK
 * 3) for each module, copies it into its new location, starting at
 *    KANETON_MODULES
 *
 * this function also updates the kaneton memory structure especially
 * modules and areas.
 *
 * each module is followed in main memory by its name.
 */

t_vaddr			bootloader_relocate(multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		khdr = (Elf32_Ehdr*)mod[0].mod_start;
  t_paddr		addr = KANETON_MODULES;
  t_uint32		modsz;
  t_uint32		i;

  cons_msg('+', "relocating modules...\n");

  /*
   * 1)
   */

  modsz = mod[0].mod_end - mod[0].mod_start;

  if (modsz > KANETON_KERNEL_CODESZ)
    {
      cons_msg('!', "error: the kernel size is over two megabytes\n");
      bootloader_error();
    }

  memcpy((void*)KANETON_KERNEL_CODE, (const void*)mod[0].mod_start, modsz);
  strcpy((char*)(KANETON_KERNEL_CODE + modsz), (const char*)mod[0].string);

  kaneton.modules[0].name = (char*)(KANETON_KERNEL_CODE + modsz);
  kaneton.modules[0].address = KANETON_KERNEL_CODE;
  kaneton.modules[0].size = modsz;

  kaneton.areas[3].address = KANETON_KERNEL_CODE;
  kaneton.areas[3].size = KANETON_KERNEL_CODESZ;

  cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
	   kaneton.modules[0].name, mod[0].mod_start,
	   kaneton.modules[0].address, kaneton.modules[0].size);

  /*
   * 2)
   *
   * nothing to do here.
   */

  kaneton.areas[4].address = KANETON_KERNEL_STACK;
  kaneton.areas[4].size = KANETON_KERNEL_STACKSZ;

  /*
   * 3)
   */

  for (i = 1; i < mbi->mods_count; i++)
    {
      t_uint32		length = strlen((char*)mod[i].string);

      modsz = mod[i].mod_end - mod[i].mod_start;

      memcpy((void*)addr, (const void*)mod[i].mod_start, modsz);
      strcpy((char*)(addr + modsz), (const char*)mod[i].string);

      kaneton.modules[i].name = (char*)(addr + modsz);
      kaneton.modules[i].address = addr;
      kaneton.modules[i].size = modsz;

      cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
	       kaneton.modules[i].name, mod[i].mod_start,
	       kaneton.modules[i].address, kaneton.modules[i].size);

      addr += ((modsz + length) / PAGESZ +
	       (((modsz + length) % PAGESZ) ? 1 : 0)) * PAGESZ;

      kaneton.areas[4 + i].address = kaneton.modules[i].address;
      kaneton.areas[4 + i].size = addr - kaneton.modules[i].address;
    }

  return (khdr->e_entry);
}

/*
 * this function dumps the kaneton memory structure
 */

void			bootloader_kaneton_dump(void)
{
  t_uint32		i;

  cons_msg('+', "dumping kaneton memory structure\n");

  cons_msg('+', " memory: %u bytes\n", kaneton.size);
  cons_msg('+', " modules: %u\n", kaneton.nmodules);

  for (i = 0; i < kaneton.nmodules; i++)
    cons_msg('+', "  [%u] %s 0x%x (0x%x)\n",
	     i,
	     kaneton.modules[i].name,
	     kaneton.modules[i].address,
	     kaneton.modules[i].size);
  /*
  cons_msg('+', " areas: %u\n", kaneton.nareas);

  for (i = 0; i < kaneton.nareas; i++)
    cons_msg('+', "  [%u] 0x%x (0x%x)\n", i,
	     kaneton.areas[i].address,
	     kaneton.areas[i].size);
  */
}

/*
 * the bootloader entry point.
 *
 * the booloder:
 *
 * 1) initializes the kaneton memory structure
 * 2) relocates binaries, data, stack
 * 3) installs the protected mode
 * 4) installs the paging mode
 * 5) update registers for the new kernel stack
 * 6) then, the kernel is launched
 * 7) this part is only reached if the kernel exit
 *
 * note that areas are composed of:
 *  [0] ISA
 *  [1] GDT
 *  [2] PD and PTs
 *  [3] kernel code
 *  [4] kernel stack
 *  [5-] the modules
 */

void bande()
{

}

int			bootloader(t_uint32		magic,
				   multiboot_info_t*	mbi)
{
  t_uint32		nareas = 5 + mbi->mods_count;
  t_area		areas[nareas];
  t_uint32		nmodules = mbi->mods_count;
  t_module		modules[nmodules];

  cons_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  printf("\n                      --- the kaneton operating system ---\n\n");

  /*
   * 1)
   */

  kaneton.start = 0;
  kaneton.size = mbi->mem_upper * 1024;
  kaneton.modules = modules;
  kaneton.nmodules = nmodules;
  kaneton.areas = areas;
  kaneton.nareas = nareas;

  kaneton.areas[0].address = 0x0;
  kaneton.areas[0].size = 0x000fffff;

  /*
   * 2)
   */

  kernel = (void (*)(t_kaneton*))bootloader_relocate(mbi);

  /*
   * 3)
   */

  pmode_init();

  /*
   * 4)
   */

  paging_init();

  /*
   * 5)
   */

  asm volatile ("movl %%ebp, %0\n"
		"movl %%esp, %1\n"
		: "=m" (ebp), "=m" (esp)
		:);

  asm volatile ("movl %0, %%ebp\n"
		"movl %0, %%esp\n"
		"pushl %1\n"
		:
		: "g" (KANETON_KERNEL_STACK + KANETON_KERNEL_STACKSZ - 1),
		  "g" (&kaneton));

  /*
   * 6)
   */

  kernel(&kaneton);

  asm volatile ("movl %0, %%ebp\n"
		"movl %1, %%esp\n"
		:
		: "g" (ebp), "g" (esp));

  /*
   * 7)
   */

  cons_msg('!', "error: kernel exited\n");
  bootloader_error();
}
