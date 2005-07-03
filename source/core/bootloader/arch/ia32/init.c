/*
 * kaneton
 *
 * init.c
 *
 * path          /home/mycure/kaneton
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 20:43:14 2004   mycure
 * last update   Sun Jul  3 15:21:46 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * init variable, describing the initial memory layout.
 *
 * this structure will be passed to the kernel describing modules,
 * physical memory reserved by the hardware etc.. thus the kernel will
 * be able to initialize its managers: segment manager, module manager etc..
 */

t_init*			init;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the init structure
 */

#if (IA32_DEBUG & IA32_DEBUG_INIT)
void			bootloader_init_dump(void)
{
  t_module*		module;
  t_uint32		i;

  bootloader_cons_msg('#', "dumping memory structure\n");

  bootloader_cons_msg('#', " memory: 0x%x - %u bytes\n",
		      init->mem,
		      init->memsz);

  bootloader_cons_msg('#', " kernel code: 0x%x - %u bytes\n",
		      init->kcode,
		      init->kcodesz);

  bootloader_cons_msg('#', " init structure: 0x%x - %u bytes\n",
		      init,
		      sizeof(t_init));

  bootloader_cons_msg('#', " modules: 0x%x - %u bytes\n",
		      init->modules,
		      init->modulessz);

  for (i = 0, module = (t_module*)((t_uint32)init->modules +
				   sizeof(t_modules));
       i < init->modules->nmodules;
       i++)
    {
      bootloader_cons_msg('#', "  [%u] %s 0x%x (0x%x)\n",
			  i,
			  module->name,
			  module,
			  module->size);

      module = (t_module*)((t_uint32)module + sizeof(t_module) +
			   module->size + strlen(module->name) + 1);
    }

  bootloader_cons_msg('#', " segments: 0x%x - %u bytes\n",
		      init->segments,
		      init->segmentssz);

  for (i = 0; i < init->nsegments; i++)
    bootloader_cons_msg('#', "  [%u] 0x%x (0x%x)\n",
			i,
			init->segments[i].address,
			init->segments[i].size);

  bootloader_cons_msg('#', " regions: 0x%x - %u bytes\n",
		      init->regions,
		      init->regionssz);

  for (i = 0; i < init->nregions; i++)
    bootloader_cons_msg('#', "  [%u] 0x%x\n",
			i,
			init->regions[i].address);

  bootloader_cons_msg('#', " kernel stack: 0x%x - %u bytes\n",
		      init->kstack,
		      init->kstacksz);

  bootloader_cons_msg('#', " alloc pages: 0x%x - %u bytes\n",
		      init->alloc,
		      init->allocsz);

  bootloader_cons_msg('#', " %#~ia32%# global offset table: 0x%x\n",
		      CONS_FRONT(CONS_CYAN) | CONS_BACK(CONS_BLACK) |
		      CONS_INT,
		      CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) |
		      CONS_INT,
		      init->machdep.gdt);

  bootloader_cons_msg('#', " %#~ia32%# page directory: 0x%x\n",
		      CONS_FRONT(CONS_CYAN) | CONS_BACK(CONS_BLACK) |
		      CONS_INT,
		      CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) |
		      CONS_INT,
		      init->machdep.pd);
}
#endif

/*
 * this function adds the segments to the init variable.
 *
 * steps:
 *
 * 1) allocates memory for the alloc() function. indeed the alloc() function
 *    needs to be able to provide memory until the virtual memory manager
 *    is installed. thus the alloc() function will be able to ask
 *    the virtual memory manager to provide virtual pages and to map it
 *    into the kernel address space to continue to provide its service.
 *    so the alloc() function needs an amount of critical pages to work with
 *    until the virtual memory is initialized.
 * 2) adds the ISA segment from 0 to 1Mb.
 * 3) adds the kernel code segment.
 * 4) adds the init structure segment.
 * 5) adds the modules segment.
 * 6) adds the segments segment.
 * 7) adds the regions segment.
 * 8) adds the kernel stack segment.
 * 9) adds the alloc segment.
 * 10) adds the global offset table segment.
 * 11) adds the page directory segment.
 */

void			bootloader_init_segments(void)
{
  t_psize		allocsz;

  /*
   * 1)
   */

  /*
   * XXX temporary, pre-allocate four pages
   */

  init->alloc = bootloader_init_alloc(4 * PAGESZ, &allocsz);
  init->allocsz = allocsz;

  /*
   * 2)
   */

  init->segments[0].address = 0x0;
  init->segments[0].size = 0x00100000;

  /*
   * 3)
   */

  init->segments[1].address = init->kcode;
  init->segments[1].size = init->kcodesz;

  /*
   * 4)
   */

  init->segments[2].address = init->init;
  init->segments[2].size = init->initsz;

  /*
   * 5)
   */

  init->segments[3].address = (t_paddr)init->modules;
  init->segments[3].size = init->modulessz;

  /*
   * 6)
   */

  init->segments[4].address = (t_paddr)init->segments;
  init->segments[4].size = init->segmentssz;

  /*
   * 7)
   */

  init->segments[5].address = (t_paddr)init->regions;
  init->segments[5].size = init->regionssz;

  /*
   * 8)
   */

  init->segments[6].address = init->kstack;
  init->segments[6].size = init->kstacksz;

  /*
   * 9)
   */

  init->segments[7].address = init->alloc;
  init->segments[7].size = init->allocsz;

  /*
   * 10)
   */

  init->segments[8].address = (t_paddr)init->machdep.gdt;
  init->segments[8].size = PAGESZ;

  /*
   * 11)
   */

  init->segments[9].address = (t_paddr)init->machdep.pd;
  init->segments[9].size = PAGESZ;
}

/*
 * this function adds the regions to the init variable.
 *
 * steps:
 *
 * 1) adds the ISA region.
 * 2) adds the kernel code region.
 * 3) adds the init structure region.
 * 4) adds the kernel stack region.
 * 5) adds the alloc region.
 * 6) adds the global offset table region.
 * 7) adds the page directory region.
 */

void			bootloader_init_regions(void)
{
  /*
   * 1)
   */

  init->regions[0].address = init->segments[0].address;

  /*
   * 2)
   */

  init->regions[1].address = init->segments[1].address;

  /*
   * 3)
   */

  init->regions[2].address = init->segments[2].address;

  /*
   * 4)
   */

  init->regions[3].address = init->segments[6].address;

  /*
   * 5)
   */

  init->regions[4].address = init->segments[7].address;

  /*
   * 6)
   */

  init->regions[5].address = init->segments[8].address;

  /*
   * 7)
   */

  init->regions[6].address = init->segments[9].address;
}

/*
 * this function just allocates memory for relocation.
 *
 * this function is needed because many objects are dynamic: modules,
 * page tables etc..
 */

t_paddr			bootloader_init_alloc(t_psize		size,
					      t_psize*		psize)
{
  static t_paddr	relocate = INIT_RELOCATE;
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
 * this is not necessary but we wanted to do it properly. moreover this is
 * not entirely useless because the DMA needs the sixteen lowest megabytes.
 *
 * note that the first module will be considered as the kernel module,
 * as multibootloaders do.
 *
 * steps:
 *
 * 1) allocates space for the kernel code. this memory is very special
 *    because it always be located at the address INIT_RELOCATE.
 *    indeed, the kernel is especially compiled to run at this address.
 * 2) allocates and initialize memory for the init structure
 *    which will be passed to the kernel.
 * 3) computes entire size of the modules.
 * 4) initializes the main fields of the init structure.
 * 5) copies the kernel code into its new location.
 * 6) initializes the kernel stack.
 * 7) for each module, copies it into its new location.
 */

t_vaddr			bootloader_init_relocate(multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		khdr = (Elf32_Ehdr*)mod->mod_start;
  t_uint32		nmodules = mbi->mods_count;
  t_uint32		nsegments = INIT_SEGMENTS;
  t_uint32		nregions = INIT_REGIONS;
  t_psize		modulessz;
  t_psize		segmentssz;
  t_psize		regionssz;
  t_paddr		kcode;
  t_psize		kcodesz;
  t_module*		module;
  t_psize		initsz;
  t_psize		modsz;
  t_uint32		i;

  bootloader_cons_msg('+', "relocating kernel and modules...\n");

  /*
   * 1)
   */

  kcode = bootloader_init_alloc(mod->mod_end - mod->mod_start, &kcodesz);

  if (kcode != INIT_RELOCATE)
    {
      bootloader_cons_msg('!', "error: kernel exited\n");
      bootloader_error();
    }

  /*
   * 2)
   */

  init = (t_init*)bootloader_init_alloc(sizeof(t_init), &initsz);
  memset(init, 0x0, sizeof(t_init));

  /*
   * 3)
   */

  for (modulessz = 0, i = 0; i < nmodules; i++)
    modulessz += mod[i].mod_end - mod[i].mod_start +
      strlen((char*)mod[i].string) + 1;

  /*
   * 4)
   */

  init->mem = 0;
  init->memsz = mbi->mem_upper * 1024;
  init->init = (t_paddr)init;
  init->initsz = initsz;
  init->kcode = kcode;
  init->kcodesz = kcodesz;

  init->modules =
    (t_modules*)bootloader_init_alloc(sizeof(t_modules) +
				      nmodules * sizeof(t_module) +
				      modulessz,
				      &modulessz);
  memset(init->modules, 0x0, modulessz);
  init->modulessz = modulessz;
  init->modules->nmodules = nmodules;

  init->segments =
    (o_segment*)bootloader_init_alloc(nsegments * sizeof(o_segment),
				       &segmentssz);
  memset(init->segments, 0x0, segmentssz);
  init->nsegments = nsegments;
  init->segmentssz = segmentssz;

  init->regions =
    (o_region*)bootloader_init_alloc(nregions * sizeof(o_region),
				      &regionssz);
  memset(init->regions, 0x0, regionssz);
  init->nregions = nregions;
  init->regionssz = regionssz;

  /*
   * 5)
   */

  modsz = mod->mod_end - mod->mod_start;
  memcpy((void*)init->kcode, (const void*)mod[0].mod_start, modsz);

  bootloader_cons_msg('+', " kernel relocated from 0x%x to 0x%x (0x%x)\n",
		      mod[0].mod_start, init->kcode, modsz);

  /*
   * 6)
   */

  init->kstack = bootloader_init_alloc(INIT_KSTACKSZ, NULL);
  init->kstacksz = INIT_KSTACKSZ;

  /*
   * 7)
   */

  for (i = 0,
	 module = (t_module*)((t_uint32)init->modules + sizeof(t_modules));
       i < init->modules->nmodules;
       i++)
    {
      t_uint32		length = strlen((char*)mod[i].string);

      modsz = mod[i].mod_end - mod[i].mod_start;

      module->name = (char*)((t_uint32)module + sizeof(t_module) + modsz);
      strcpy(module->name, (char*)mod[i].string);

      memcpy((void*)((t_uint32)module + sizeof(t_module)),
	     (const void*)mod[i].mod_start, modsz);

      module->size = modsz;

      bootloader_cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
			  module->name,
			  mod[i].mod_start,
			  module,
			  module->size);

      module = (t_module*)((t_uint32)module + sizeof(t_module) +
			   module->size + strlen(module->name) + 1);
    }

  return (khdr->e_entry);
}
