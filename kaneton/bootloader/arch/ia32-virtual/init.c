/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/bootloader/arch/ia32-virtual/init.c
 *
 * created       julien quintard   [mon jul 19 20:43:14 2004]
 * updated       julien quintard   [thu aug 31 20:18:30 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * init variable, describing the initial memory layout.
 *
 * this structure will be passed to the kernel describing modules,
 * physical memory reserved by the hardware etc.. thus the kernel will
 * be able to initialise its managers: segment manager, module manager etc..
 */

t_init*			init;

/*
 * XXX
 */

static t_paddr		relocate;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k1 */

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

  bootloader_cons_msg('#', " cpus: 0x%x - %u bytes\n",
		      init->cpus,
		      init->cpussz);

  for (i = 0; i < init->ncpus; i++)
    bootloader_cons_msg('#', "  [%u]\n",
			i);

  bootloader_cons_msg('#', " kernel stack: 0x%x - %u bytes\n",
		      init->kstack,
		      init->kstacksz);

  bootloader_cons_msg('#', " alloc pages: 0x%x - %u bytes\n",
		      init->alloc,
		      init->allocsz);

  bootloader_cons_msg('#', " %#~ia32%# global offset table: 0x%x (%u bytes)\n",
		      CONS_FRONT(CONS_CYAN) | CONS_BACK(CONS_BLACK) |
		      CONS_INT,
		      CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) |
		      CONS_INT,
		      init->machdep.gdt.addr,
		      init->machdep.gdt.size);

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
 * 1) adds the ISA segment from 0 to 1Mb.
 * 2) adds the kernel code segment.
 * 3) adds the init structure segment.
 * 4) adds the modules segment.
 * 5) adds the segments segment.
 * 6) adds the regions segment.
 * 7) adds the cpu segment.
 * 8) adds the kernel stack segment.
 * 9) adds the alloc segment.
 * 10) adds the global offset table segment.
 * 11) adds the page directory segment.
 */

void			bootloader_init_segments(void)
{
  /*
   * 1)
   */

  init->segments[0].address = INIT_ISA_ADDR;
  init->segments[0].size = 0x1000;
  init->segments[0].perms = PERM_READ;
  init->segments[0].type = SEGMENT_TYPE_MEMORY;

  init->segments[1].address = INIT_ISA_ADDR + 0x1000;
  init->segments[1].size = INIT_ISA_SIZE - 0x1000;
  init->segments[1].perms = PERM_READ | PERM_WRITE;
  init->segments[1].type = SEGMENT_TYPE_MEMORY;

  /*
   * 2)
   */

  init->segments[2].address = init->kcode;
  init->segments[2].size = init->kcodesz;
  init->segments[2].perms = PERM_READ | PERM_WRITE | PERM_EXEC;
  init->segments[2].type = SEGMENT_TYPE_MEMORY;

  /*
   * 3)
   */

  init->segments[3].address = init->init;
  init->segments[3].size = init->initsz;
  init->segments[3].perms = PERM_READ | PERM_WRITE;
  init->segments[3].type = SEGMENT_TYPE_MEMORY;

  /*
   * 4)
   */

  init->segments[4].address = (t_paddr)init->modules;
  init->segments[4].size = init->modulessz;
  init->segments[4].perms = PERM_READ | PERM_WRITE | PERM_EXEC;
  init->segments[4].type = SEGMENT_TYPE_MEMORY;

  /*
   * 5)
   */

  init->segments[5].address = (t_paddr)init->segments;
  init->segments[5].size = init->segmentssz;
  init->segments[5].perms = PERM_READ | PERM_WRITE;
  init->segments[5].type = SEGMENT_TYPE_MEMORY;

  /*
   * 6)
   */

  init->segments[6].address = (t_paddr)init->regions;
  init->segments[6].size = init->regionssz;
  init->segments[6].perms = PERM_READ | PERM_WRITE;
  init->segments[6].type = SEGMENT_TYPE_MEMORY;

  /*
   * 7)
   */

  init->segments[7].address = (t_paddr)init->cpus;
  init->segments[7].size = init->cpussz;
  init->segments[7].perms = PERM_READ | PERM_WRITE;
  init->segments[7].type = SEGMENT_TYPE_MEMORY;

  /*
   * 8)
   */

  init->segments[8].address = init->kstack;
  init->segments[8].size = init->kstacksz;
  init->segments[8].perms = PERM_READ | PERM_WRITE;
  init->segments[8].type = SEGMENT_TYPE_MEMORY;

  /*
   * 9)
   */

  init->segments[9].address = init->alloc;
  init->segments[9].size = init->allocsz;
  init->segments[9].perms = PERM_READ | PERM_WRITE;
  init->segments[9].type = SEGMENT_TYPE_MEMORY;

  /*
   * 10)
   */

  /* XXX */
  init->segments[10].address = (t_paddr)init->machdep.gdt.descriptor;
  init->segments[10].size = PAGESZ/*init->machdep.gdt.count * sizeof (t_gdte)*/;
  init->segments[10].perms = PERM_READ | PERM_WRITE;
  init->segments[10].type = SEGMENT_TYPE_MEMORY;

  /*
   * 11)
   */

  init->segments[11].address = (t_paddr)init->machdep.pd;
  init->segments[11].size = PAGESZ;
  init->segments[11].perms = PERM_READ | PERM_WRITE;
  init->segments[11].type = SEGMENT_TYPE_MEMORY;
}

/*
 * this function adds the regions to the init variable.
 *
 * steps:
 *
 * 1) add the ISA region.
 * 2) add the kernel code region.
 * 3) add the init structure region.
 * 4) add the segments region.
 * 5) add the region region.
 * 6) add the cpu region.
 * 7) add the kernel stack region.
 * 8) add the alloc region.
 * 9) add the global offset table region.
 * 10) add the page directory region.
 */

void			bootloader_init_regions(void)
{
  /*
   * 1)
   */

  init->regions[0].address = init->segments[1].address;
  init->regions[0].size = init->segments[1].size;
  init->regions[0].offset = 0;
  init->regions[0].segid = 1;
  init->regions[0].opts = REGION_OPT_PRIVILEGED;

  /*
   * 2)
   */

  init->regions[1].address = init->segments[2].address;
  init->regions[1].size = init->segments[2].size;
  init->regions[1].offset = 0;
  init->regions[1].segid = 2;
  init->regions[1].opts = REGION_OPT_PRIVILEGED;

  /*
   * 3)
   */

  init->regions[2].address = init->segments[3].address;
  init->regions[2].size = init->segments[3].size;
  init->regions[2].offset = 0;
  init->regions[2].segid = 3;
  init->regions[2].opts = REGION_OPT_PRIVILEGED;

  /*
   * 4)
   */

  init->regions[3].address = init->segments[5].address;
  init->regions[3].size = init->segments[5].size;
  init->regions[3].offset = 0;
  init->regions[3].segid = 5;
  init->regions[3].opts = REGION_OPT_PRIVILEGED;

  /*
   * 5)
   */

  init->regions[4].address = init->segments[6].address;
  init->regions[4].size = init->segments[6].size;
  init->regions[4].offset = 0;
  init->regions[4].segid = 6;
  init->regions[4].opts = REGION_OPT_PRIVILEGED;


  /*
   * 6)
   */

  init->regions[5].address = init->segments[7].address;
  init->regions[5].size = init->segments[7].size;
  init->regions[5].offset = 0;
  init->regions[5].segid = 7;
  init->regions[5].opts = REGION_OPT_PRIVILEGED;

  /*
   * 7)
   */

  init->regions[6].address = init->segments[8].address;
  init->regions[6].size = init->segments[8].size;
  init->regions[6].offset = 0;
  init->regions[6].segid = 8;
  init->regions[6].opts = REGION_OPT_PRIVILEGED;

  /*
   * 8)
   */

  init->regions[7].address = init->segments[9].address;
  init->regions[7].size = init->segments[9].size;
  init->regions[7].offset = 0;
  init->regions[7].segid = 9;
  init->regions[7].opts = REGION_OPT_PRIVILEGED | REGION_OPT_GLOBAL;

  /*
   * 9)
   */

  init->regions[8].address = init->segments[10].address;
  init->regions[8].size = init->segments[10].size;
  init->regions[8].offset = 0;
  init->regions[8].segid = 10;
  init->regions[8].opts = REGION_OPT_PRIVILEGED;

  /*
   * 10)
   */

  init->regions[9].address = init->segments[11].address;
  init->regions[9].size = init->segments[11].size;
  init->regions[9].offset = 0;
  init->regions[9].segid = 11;
  init->regions[9].opts = REGION_OPT_PRIVILEGED;
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
 *    because it always be located at the address init_relocate.
 *    indeed, the kernel is especially compiled to run at this address.
 * 2) allocates and initialise memory for the init structure
 *    which will be passed to the kernel.
 * 3) computes entire size of the modules.
 * 4) initialises the main fields of the init structure.
 * 5) copies the kernel code into its new location.
 * 6) initialises the kernel stack.
 * 7) for each module, copies it into its new location.
 * 8) allocates memory for the alloc() function. indeed the alloc() function
 *    needs to be able to provide memory until the virtual memory manager
 *    is installed. thus the alloc() function will be able to ask
 *    the virtual memory manager to provide virtual pages and to map it
 *    into the kernel address space to continue to provide its service.
 *    so the alloc() function needs an amount of critical pages to work with
 *    until the virtual memory is initialised.
 */

t_vaddr			bootloader_init_relocate(multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		khdr = (Elf32_Ehdr*)mod->mod_start;
  Elf32_Phdr*		phdr;
  t_paddr		init_relocate;
  t_uint32		nmodules = mbi->mods_count;
  t_uint32		nsegments = INIT_SEGMENTS;
  t_uint32		nregions = INIT_REGIONS;
  t_psize		modulessz;
  t_psize		segmentssz;
  t_psize		regionssz;
  t_psize		cpussz;
  t_paddr		kcode;
  t_psize		kcodesz;
  t_psize		allocsz;
  t_module*		module;
  t_psize		initsz;
  t_psize		modsz;
  t_uint32		i;

  bootloader_cons_msg('+', "relocating kernel and modules...\n");

  /*
   * 1)
   */

  phdr = (Elf32_Phdr*)((char*)khdr + khdr->e_phoff);

  relocate = init_relocate = phdr->p_paddr;

  kcode = bootloader_init_alloc(mod->mod_end - mod->mod_start, &kcodesz);

  if (kcode != init_relocate)
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

  init->cpus =
    (o_cpu*)bootloader_init_alloc(sizeof(o_cpu),
				      &cpussz);
  memset(init->cpus, 0x0, cpussz);
  init->ncpus = 1;
  init->cpussz = cpussz;

  init->cpus[0].cpuid = 0;
  init->bsp = 0;

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

  /*
   * 8)
   */

  init->alloc = bootloader_init_alloc(16 * PAGESZ, &allocsz);
  init->allocsz = allocsz;

  return (khdr->e_entry);
}

/*                                                                 [cut] /k1 */
