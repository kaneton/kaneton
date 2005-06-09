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
 * last update   Thu Jun  9 20:45:44 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * version string
 */

extern const char	version[];

/*
 * bootloader memory description variable.
 *
 * this structure will be passed to the kernel describing modules,
 * physical memory reserved by the hardware etc.. thus the kernel will
 * be able to initialize its managers: segments manager, modules manager etc..
 */

t_bmem*			bmem;

/*
 * these variables are useful to avoid a very very long cast
 */

t_bmods*		bmods;
t_bareas*		bareas;
t_kareas*		kareas;

/*
 * these three variable are globals because we have no choice.
 *
 * indeed, before jumping on the kernel a new stack is created so the local
 * variables will no longer be accessible. putting these variables in
 * globals bypass this problem.
 */

void			(*kernel)(t_bmem*);
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
 * this function just allocates memory for relocation.
 *
 * this function is needed because many objects are dynamic: modules, areas,
 * page tables etc..
 */

t_paddr			bootloader_alloc(t_psize		size,
					 t_psize*		psize)
{
  static t_paddr	relocate = BOOTLOADER_RELOCATE;
  t_paddr		allocated = relocate;
  t_psize		padded;

  padded = size / PAGESZ;
  padded += size % PAGESZ ? 1 : 0;

  relocate += padded * PAGESZ;

  if (psize != NULL)
    *psize = padded * PAGESZ;

  return (allocated);
}

/*
 * this function relocates the kernel and modules over the sixteen megabytes
 * and returns the kernel entry point address.
 *
 * this is not necessary but we want to do it properly. moreover this is
 * not entirely useless because the DMA needs the sixteen lowest megabytes.
 *
 * note that the first module will be considered as the kernel module
 * as multibootloaders do.
 *
 * the function:
 *
 * 1) allocates space for the kernel code. this memory is very special
 *    because it always be located at the address BOOTLOADER_RELOCATE.
 *    indeed, the kernel is especially compiled to run with this address.
 * 2) allocates and initialize memory for the bootloader memory structure
 *    which will be passed to the kernel.
 * 3) initialize the main fields of the bootloader memory structure.
 * 4) copies the kernel code into its new location.
 * 5) initializes the kernel stack.
 * 6) for each module, copies it into its new location followed by its
 *    name in main memory.
 *
 * this function also updates the bootloader memory structure especially
 * modules and areas.
 */

t_vaddr			bootloader_relocate(multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		khdr = (Elf32_Ehdr*)mod->mod_start;
  t_uint32		nbmods = mbi->mods_count;
  t_uint32		nbareas = 5 + mbi->mods_count;
  t_uint32		nkareas = 0; /* XXX */
  t_psize		bmodssz;
  t_psize		bareassz;
  t_psize		kareassz;
  t_paddr		kcode;
  t_psize		kcodesz;
  t_psize		modsz;
  t_paddr		addr;
  t_uint32		i;

  cons_msg('+', "relocating kernel and modules...\n");

  /*
   * 1)
   */

  kcode = bootloader_alloc(mod->mod_end - mod->mod_start, &kcodesz);

  if (kcode != BOOTLOADER_RELOCATE)
    {
      cons_msg('!', "error: kernel exited\n");
      bootloader_error();
    }

  /*
   * 2)
   */

  bmem = (t_bmem*)bootloader_alloc(sizeof(t_bmem), NULL);
  memset(bmem, 0x0, sizeof(t_bmem));

  /*
   * 3)
   */

  bmem->mem = 0;
  bmem->memsz = mbi->mem_upper * 1024;
  bmem->kcode = kcode;
  bmem->kcodesz = kcodesz;

  bmem->bmods = bootloader_alloc(sizeof(t_bmods) + nbmods * sizeof(t_bmod),
				 &bmodssz);
  bmem->bmodssz = bmodssz;
  ((t_bmods*)bmem->bmods)->nbmods = nbmods;
  ((t_bmods*)bmem->bmods)->bmods = (t_bmod*)(bmem->bmods + sizeof(t_bmods));

  bmem->bareas = bootloader_alloc(sizeof(t_bareas) + nbareas * sizeof(t_barea),
				  &bareassz);
  bmem->bareassz = bareassz;
  ((t_bareas*)bmem->bareas)->nbareas = nbareas;
  ((t_bareas*)bmem->bareas)->bareas = ((t_barea*)bmem->bareas +
				       sizeof(t_bareas));

  bmem->kareas = bootloader_alloc(sizeof(t_kareas) + nkareas * sizeof(t_karea),
				  &kareassz);
  bmem->kareassz = kareassz;
  ((t_kareas*)bmem->kareas)->nkareas = nkareas;
  ((t_kareas*)bmem->kareas)->kareas = ((t_karea*)bmem->kareas +
				       sizeof(t_kareas));

  bmods = (t_bmods*)(bmem->bmods);
  bareas = (t_bareas*)(bmem->bareas);
  kareas = (t_kareas*)(bmem->kareas);

  /* ISA XXX
  bmem.areas[0].address = 0x0;
  bmem.areas[0].size = 0x000fffff;
  */

  /*
   * 4)
   */

  modsz = mod->mod_end - mod->mod_start;
  memcpy((void*)bmem->kcode, (const void*)mod[0].mod_start, modsz);

  cons_msg('+', " kernel relocated from 0x%x to 0x%x (0x%x)\n",
	   mod[0].mod_start, bmem->kcode, modsz);

  /*
   * 5)
   */

  bmem->kstack = bootloader_alloc(BOOTLOADER_KSTACKSZ, NULL);
  bmem->kstacksz = BOOTLOADER_KSTACKSZ;

  /*
   * 6)
   */

  for (i = 0; i < bmods->nbmods; i++)
    {
      t_uint32		length = strlen((char*)mod[i].string);

      modsz = mod[i].mod_end - mod[i].mod_start;

      addr = bootloader_alloc(modsz + length, NULL);

      memcpy((void*)addr, (const void*)mod[i].mod_start, modsz);
      strcpy((char*)(addr + modsz), (const char*)mod[i].string);

      bmods->bmods[i].name = (char*)(addr + modsz);
      bmods->bmods[i].address = addr;
      bmods->bmods[i].size = modsz;

      cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
	       bmods->bmods[i].name, mod[i].mod_start,
	       bmods->bmods[i].address, bmods->bmods[i].size);
    }

  return (khdr->e_entry);
}

/*
 * this function dumps the bootloader memory structure
 */

#if (IA32_DEBUG & IA32_DEBUG_BOOTLOADER)
void			bootloader_dump(void)
{
  t_bmods*		bmods = (t_bmods*)bmem->bmods;
  t_bareas*		bareas = (t_bareas*)bmem->bareas;
  t_kareas*		kareas = (t_kareas*)bmem->kareas;
  t_uint32		i;

  cons_msg('#', "dumping memory structure\n");

  cons_msg('#', " memory: 0x%x - %u bytes\n",
	   bmem->mem,
	   bmem->memsz);

  cons_msg('#', " kernel code: 0x%x - %u bytes\n",
	   bmem->kcode,
	   bmem->kcodesz);

  cons_msg('#', " bootloader memory structure: 0x%x - %u bytes\n",
	   bmem,
	   sizeof(t_bmem));

  cons_msg('#', " kernel stack: 0x%x - %u bytes\n",
	   bmem->kstack,
	   bmem->kstacksz);

  cons_msg('#', " bootloader modules: 0x%x - %u bytes\n",
	   bmem->bmods,
	   bmem->bmodssz);

  for (i = 0; i < bmods->nbmods; i++)
    cons_msg('#', "  [%u] %s 0x%x (0x%x)\n",
	     i,
	     bmods->bmods[i].name,
	     bmods->bmods[i].address,
	     bmods->bmods[i].size);

  cons_msg('#', " bootloader areas: 0x%x - %u bytes\n",
	   bmem->bareas,
	   bmem->bareassz);

  for (i = 0; i < bareas->nbareas; i++)
    cons_msg('#', "  [%u] 0x%x (0x%x)\n",
	     i,
	     bareas->bareas[i].address,
	     bareas->bareas[i].size);

  cons_msg('#', " kernel areas: 0x%x - %u bytes\n",
	   bmem->kareas,
	   bmem->kareassz);

  for (i = 0; i < kareas->nkareas; i++)
    cons_msg('#', "  [%u] 0x%x (0x%x)\n",
	     i,
	     kareas->kareas[i].address,
	     kareas->kareas[i].size);

  cons_msg('#', " global offset table: 0x%x\n", bmem->machdep.gdt);

  cons_msg('#', " page directory: 0x%x\n", bmem->machdep.pd);

  cons_msg('#', " console: 0x%x\n", bmem->machdep.cons);
}
#endif

/*
 * the bootloader entry point.
 *
 * the booloder:
 *
 * 1) initializes the bootloader memory structure
 * 2) relocates binaries, data, stack
 * 3) installs the protected mode
 * 4) installs the paging mode
 * 5) loads the console state for the kernel.
 * 6) update registers for the new kernel stack
 * 7) then, the kernel is launched
 * 8) this part is only reached if the kernel exit
 *
 * note that areas are composed of:
 *  [0] ISA
 *  [1] GDT
 *  [2] PD and PTs
 *  [3] kernel code
 *  [4] kernel stack
 *  [5-] the modules
 */

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi)
{
  cons_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  printf("\n                      --- the kaneton operating system ---\n\n");

  cons_msg('+', "%s\n\n", version);

  /*
   * 1)
   */

  /*
   * 2)
   */

  kernel = (void (*)(t_bmem*))bootloader_relocate(mbi);

  /*
   * 3)
   */

  pmode_init();

  /*
   * 4)
   */

  paging_init();

#if (IA32_DEBUG & IA32_DEBUG_BOOTLOADER)
  bootloader_dump();
#endif

  /*
   * 5)
   */

  cons_load();

  /*
   * 6)
   */

  asm volatile ("movl %%ebp, %0\n"
		"movl %%esp, %1\n"
		: "=m" (ebp), "=m" (esp)
		:);
  /* XXX
  asm volatile ("movl %0, %%ebp\n"
		"movl %0, %%esp\n"
		"pushl %1\n"
		:
		: "g" (BOOTLOADER_KSTACK + BOOTLOADER_KSTACKSZ - 1),
		  "g" (&bmem));
  */
  /*
   * 7)
   */

  /* XXX kernel(&bmem); */

  asm volatile ("movl %0, %%ebp\n"
		"movl %1, %%esp\n"
		:
		: "g" (ebp), "g" (esp));

  /*
   * 8)
   */

  cons_msg('!', "error: kernel exited\n");
  bootloader_error();
}
