/*
 * kaneton
 *
 * init.c
 *
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 20:43:14 2004   mycure
 * last update   Thu Jun 16 15:57:55 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * init variable, describing the initial memory layout.
 *
 * this structure will be passed to the kernel describing modules,
 * physical memory reserved by the hardware etc.. thus the kernel will
 * be able to initialize its managers: segment manager, module manager etc..
 */

t_init*			init;

/*
 * this function dumps the init structure
 */

#if (IA32_DEBUG & IA32_DEBUG_INIT)
void			init_dump(void)
{
  t_module*		module;
  t_uint32		i;

  cons_msg('#', "dumping memory structure\n");

  cons_msg('#', " memory: 0x%x - %u bytes\n",
	   init->mem,
	   init->memsz);

  cons_msg('#', " kernel code: 0x%x - %u bytes\n",
	   init->kcode,
	   init->kcodesz);

  cons_msg('#', " init structure: 0x%x - %u bytes\n",
	   init,
	   sizeof(t_init));

  cons_msg('#', " modules: 0x%x - %u bytes\n",
	   init->modules,
	   init->modulessz);

  for (i = 0, module = (t_module*)((t_uint32)init->modules +
				   sizeof(t_modules));
       i < init->modules->nmodules;
       i++)
    {
      cons_msg('#', "  [%u] %s 0x%x (0x%x)\n",
	       i,
	       module->name,
	       module,
	       module->size);

      module = (t_module*)((t_uint32)module + sizeof(t_module) + module->size);
    }

  cons_msg('#', " segments: 0x%x - %u bytes\n",
	   init->segments,
	   init->segmentssz);

  for (i = 0; i < init->segments->nsegments; i++)
    cons_msg('#', "  [%u] 0x%x (0x%x)\n",
	     i,
	     init->segments->segments[i].address,
	     init->segments->segments[i].size);

  cons_msg('#', " regions: 0x%x - %u bytes\n",
	   init->regions,
	   init->regionssz);

  for (i = 0; i < init->regions->nregions; i++)
    cons_msg('#', "  [%u] 0x%x\n",
	     i,
	     init->regions->regions[i].address);

  cons_msg('#', " kernel stack: 0x%x - %u bytes\n",
	   init->kstack,
	   init->kstacksz);

  cons_msg('#', " segment manager: 0x%x - %u bytes\n",
	   init->segmng,
	   init->segmngsz);

  cons_msg('#', " address space manager: 0x%x - %u bytes\n",
	   init->asmng,
	   init->asmngsz);

  cons_msg('#', " %#~ia32%# global offset table: 0x%x\n",
	   CONS_FRONT(CONS_CYAN) | CONS_BACK(CONS_BLACK) | CONS_INT,
	   CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) | CONS_INT,
	   init->machdep.gdt);

  cons_msg('#', " %#~ia32%# page directory: 0x%x\n",
	   CONS_FRONT(CONS_CYAN) | CONS_BACK(CONS_BLACK) | CONS_INT,
	   CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) | CONS_INT,
	   init->machdep.pd);
}
#endif

/*
 * this function adds the segments to the init variable.
 *
 * steps:
 *
 * 1) allocates memory for the segment manager.
 * 2) allocates memory for the address space manager.
 * 3) adds the ISA segment from 0 to 1Mb.
 * 4) adds the kernel code segment.
 * 5) adds the init structure segment.
 * 6) adds the modules segment.
 * 7) adds the segments segment.
 * 8) adds the regions segment.
 * 9) adds the kernel stack segment.
 * 10) adds the segment manager segment.
 * 11) adds the address space manager segment.
 * 12) adds the global offset table segment.
 * 13) adds the page directory segment.
 */

void			init_segments(void)
{
  t_psize		segmngsz;
  t_psize		asmngsz;

  /*
   * 1)
   */

  /*
   * XXX allocates a number of pages in relation with the number of
   * segments: init->segments->nsegments.
   *
   * XXX for the moment we preallocate four pages.
   */

  init->segmng = init_alloc(4 * PAGESZ, &segmngsz);
  init->segmngsz = segmngsz;

  /*
   * 2)
   */

  /*
   * XXX for the moment we preallocate 2 pages for the as manager.
   */

  init->asmng = init_alloc(2 * PAGESZ, &asmngsz);
  init->asmngsz = asmngsz;

  /*
   * 3)
   */

  init->segments->segments[0].address = 0x0;
  init->segments->segments[0].size = 0x00100000;

  /*
   * 4)
   */

  init->segments->segments[1].address = init->kcode;
  init->segments->segments[1].size = init->kcodesz;

  /*
   * 5)
   */

  init->segments->segments[2].address = init->init;
  init->segments->segments[2].size = init->initsz;

  /*
   * 6)
   */

  init->segments->segments[3].address = (t_paddr)init->modules;
  init->segments->segments[3].size = init->modulessz;

  /*
   * 7)
   */

  init->segments->segments[4].address = (t_paddr)init->segments;
  init->segments->segments[4].size = init->segmentssz;

  /*
   * 8)
   */

  init->segments->segments[5].address = (t_paddr)init->regions;
  init->segments->segments[5].size = init->regionssz;

  /*
   * 9)
   */

  init->segments->segments[6].address = init->kstack;
  init->segments->segments[6].size = init->kstacksz;

  /*
   * 10)
   */

  init->segments->segments[7].address = init->segmng;
  init->segments->segments[7].size = init->segmngsz;

  /*
   * 11)
   */

  init->segments->segments[8].address = init->asmng;
  init->segments->segments[8].size = init->asmngsz;

  /*
   * 12)
   */

  init->segments->segments[9].address = (t_paddr)init->machdep.gdt;
  init->segments->segments[9].size = PAGESZ;

  /*
   * 13)
   */

  init->segments->segments[10].address = (t_paddr)init->machdep.pd;
  init->segments->segments[10].size = PAGESZ;
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
 * 5) adds the segment manager region.
 * 6) adds the address space manager region.
 * 7) adds the global offset table region.
 * 8) adds the page directory region.
 */

void			init_regions(void)
{
  /*
   * 1)
   */

  init->regions->regions[0].address = init->segments->segments[0].address;

  /*
   * 2)
   */

  init->regions->regions[1].address = init->segments->segments[1].address;

  /*
   * 3)
   */

  init->regions->regions[2].address = init->segments->segments[2].address;

  /*
   * 4)
   */

  init->regions->regions[3].address = init->segments->segments[6].address;

  /*
   * 5)
   */

  init->regions->regions[4].address = init->segments->segments[7].address;

  /*
   * 6)
   */

  init->regions->regions[5].address = init->segments->segments[8].address;

  /*
   * 7)
   */

  init->regions->regions[6].address = init->segments->segments[9].address;

  /*
   * 8)
   */

  init->regions->regions[7].address = init->segments->segments[10].address;
}

/*
 * this function just allocates memory for relocation.
 *
 * this function is needed because many objects are dynamic: modules,
 * page tables etc..
 */

t_paddr			init_alloc(t_psize			size,
				   t_psize*			psize)
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

t_vaddr			init_relocate(multiboot_info_t*		mbi)
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

  cons_msg('+', "relocating kernel and modules...\n");

  /*
   * 1)
   */

  kcode = init_alloc(mod->mod_end - mod->mod_start, &kcodesz);

  if (kcode != INIT_RELOCATE)
    {
      cons_msg('!', "error: kernel exited\n");
      bootloader_error();
    }

  /*
   * 2)
   */

  init = (t_init*)init_alloc(sizeof(t_init), &initsz);
  memset(init, 0x0, sizeof(t_init));

  /*
   * 3)
   */

  for (modulessz = 0, i = 0; i < nmodules; i++)
    modulessz += mod[i].mod_end - mod[i].mod_start;

  /*
   * 4)
   */

  init->mem = 0;
  init->memsz = mbi->mem_upper * 1024;
  init->init = (t_paddr)init;
  init->initsz = initsz;
  init->kcode = kcode;
  init->kcodesz = kcodesz;

  init->modules = (t_modules*)init_alloc(sizeof(t_modules) +
					       nmodules * sizeof(t_module) +
					       modulessz,
					       &modulessz);
  memset(init->modules, 0x0, modulessz);
  init->modulessz = modulessz;
  init->modules->nmodules = nmodules;

  init->segments = (t_segments*)init_alloc(sizeof(t_segment) +
						 nsegments * sizeof(t_segment),
						 &segmentssz);
  memset(init->segments, 0x0, segmentssz);
  init->segmentssz = segmentssz;
  init->segments->nsegments = nsegments;
  init->segments->segments = (t_segment*)(init->segments + sizeof(t_segments));

  init->regions = (t_regions*)init_alloc(sizeof(t_regions) +
					       nregions * sizeof(t_region),
					       &regionssz);
  memset(init->regions, 0x0, regionssz);
  init->regionssz = regionssz;
  init->regions->nregions = nregions;
  init->regions->regions = (t_region*)(init->regions + sizeof(t_regions));

  /*
   * 5)
   */

  modsz = mod->mod_end - mod->mod_start;
  memcpy((void*)init->kcode, (const void*)mod[0].mod_start, modsz);

  cons_msg('+', " kernel relocated from 0x%x to 0x%x (0x%x)\n",
	   mod[0].mod_start, init->kcode, modsz);

  /*
   * 6)
   */

  init->kstack = init_alloc(INIT_KSTACKSZ, NULL);
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

      if (strlen((char*)mod[i].string) >= MOD_NAMESZ)
	cons_msg('!', "the module name %s will be truncated to %u bytes\n",
		 mod[i].string, MOD_NAMESZ);

      strncpy(module->name, (char*)mod[i].string, MOD_NAMESZ);

      memcpy((void*)((t_uint32)module + sizeof(t_module)),
	     (const void*)mod[i].mod_start, modsz);

      module->size = modsz;

      cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
	       module->name,
	       mod[i].mod_start,
	       module,
	       module->size);

      module = (t_module*)((t_uint32)module + sizeof(t_module) + module->size);
    }

  return (khdr->e_entry);
}