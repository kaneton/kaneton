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
 * last update   Wed Jun  1 11:56:45 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * version string
 */

extern const char	version[];

/*
 * memory description variable.
 *
 * this structure will be passed to the kernel describing modules,
 * physical memory reserved by the hardware etc.. thus the kernel will
 * be able to initialize its managers: segments manager, modules manager etc..
 */

t_memory		memory;

/*
 * these three variable are globals because we have no choice.
 *
 * indeed, before jumping on the kernel a new stack is created so the local
 * variables will no longer be accessible. putting these variables in
 * globals bypass this problem.
 */

void			(*kernel)(t_memory*);
t_reg32			ebp;
t_reg32			esp;

/*
 * a funny function which do nothing.
 *
 * this function is called when a fatal error occurs.
 */

void			bootloader_error(void)
{
  while (1)
    ;
}

/*
 * this function relocates the modules over the sixteen megabytes,
 * creates a kernel stack etc.. and returns the kernel entry point address.
 *
 * this is not necessary but we want to do it properly. moreover this is
 * to entirely useless because the DMA needs the sixteen lowest megabytes.
 *
 * note that the first module will be considered as the kernel module
 * as multibootloaders do.
 *
 * 1) copies the kernel code into its new location BOOTLOADER_KCODE
 * 2) initializes the kernel stack located at BOOTLOADER_KSTACK
 * 3) for each module, copies it into its new location, starting at
 *    BOOTLOADER_MODULES
 *
 * this function also updates the memory structure especially
 * modules and areas.
 *
 * note that each module is followed in main memory by its name.
 */

t_vaddr			bootloader_relocate(multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		khdr = (Elf32_Ehdr*)mod[0].mod_start;
  t_paddr		addr = BOOTLOADER_MODULES;
  t_uint32		modsz;
  t_uint32		i;

  cons_msg('+', "relocating modules...\n");

  /*
   * 1)
   */

  modsz = mod[0].mod_end - mod[0].mod_start;

  if (modsz > BOOTLOADER_KCODESZ)
    {
      cons_msg('!', "error: the kernel size is over two megabytes\n");
      bootloader_error();
    }

  memcpy((void*)BOOTLOADER_KCODE, (const void*)mod[0].mod_start, modsz);
  strcpy((char*)(BOOTLOADER_KCODE + modsz), (const char*)mod[0].string);

  memory.modules[0].name = (char*)(BOOTLOADER_KCODE + modsz);
  memory.modules[0].address = BOOTLOADER_KCODE;
  memory.modules[0].size = modsz;

  memory.areas[3].address = BOOTLOADER_KCODE;
  memory.areas[3].size = BOOTLOADER_KCODESZ;

  cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
	   memory.modules[0].name, mod[0].mod_start,
	   memory.modules[0].address, memory.modules[0].size);

  /*
   * 2)
   *
   * nothing to do here.
   */

  memory.areas[4].address = BOOTLOADER_KSTACK;
  memory.areas[4].size = BOOTLOADER_KSTACKSZ;

  /*
   * 3)
   */

  for (i = 1; i < mbi->mods_count; i++)
    {
      t_uint32		length = strlen((char*)mod[i].string);

      modsz = mod[i].mod_end - mod[i].mod_start;

      memcpy((void*)addr, (const void*)mod[i].mod_start, modsz);
      strcpy((char*)(addr + modsz), (const char*)mod[i].string);

      memory.modules[i].name = (char*)(addr + modsz);
      memory.modules[i].address = addr;
      memory.modules[i].size = modsz;

      cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
	       memory.modules[i].name, mod[i].mod_start,
	       memory.modules[i].address, memory.modules[i].size);

      addr += ((modsz + length) / PAGESZ +
	       (((modsz + length) % PAGESZ) ? 1 : 0)) * PAGESZ;

      memory.areas[4 + i].address = memory.modules[i].address;
      memory.areas[4 + i].size = addr - memory.modules[i].address;
    }

  return (khdr->e_entry);
}

/*
 * this function dumps the memory structure
 */

void			bootloader_memory_dump(void)
{
  t_uint32		i;

  cons_msg('+', "dumping memory structure\n");

  cons_msg('+', " memory: %u bytes\n", memory.size);
  cons_msg('+', " modules: %u\n", memory.nmodules);

  for (i = 0; i < memory.nmodules; i++)
    cons_msg('+', "  [%u] %s 0x%x (0x%x)\n",
	     i,
	     memory.modules[i].name,
	     memory.modules[i].address,
	     memory.modules[i].size);
  /*
  cons_msg('+', " areas: %u\n", memory.nareas);

  for (i = 0; i < memory.nareas; i++)
    cons_msg('+', "  [%u] 0x%x (0x%x)\n", i,
	     memory.areas[i].address,
	     memory.areas[i].size);
  */
}

/*
 * the bootloader entry point.
 *
 * the booloder:
 *
 * 1) initializes the memory structure
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

  cons_msg('+', "%s\n\n", version);

  /*
   * 1)
   */

  memset(&memory, 0x0, sizeof(t_memory));

  memory.start = 0;
  memory.size = mbi->mem_upper * 1024;
  memory.modules = modules;
  memory.nmodules = nmodules;
  memory.areas = areas;
  memory.nareas = nareas;

  memory.areas[0].address = 0x0;
  memory.areas[0].size = 0x000fffff;

  /*
   * 2)
   */

  kernel = (void (*)(t_memory*))bootloader_relocate(mbi);

  /*
   * 3)
   */

  pmode_init();

  /*
   * 4)
   */

  /* XXX paging_init(); */

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
		: "g" (BOOTLOADER_KSTACK + BOOTLOADER_KSTACKSZ - 1),
		  "g" (&memory));

  /*
   * 6)
   */

  /* XXX kernel(&memory); */

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
