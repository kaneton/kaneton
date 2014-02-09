/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/init.c
 *
 * created       julien quintard   [mon jul 19 20:43:14 2004]
 * updated       matthieu bucchianeri   [sun may 27 21:34:15 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "loader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * init variable, describing the initial memory layout.
 *
 * this structure will be passed to the kernel describing inputs,
 * physical memory reserved by the hardware etc. thus the kernel will
 * be able to initialize its managers: segment manager etc.
 */

t_init*			init;

/*
 * this variable is used by the page allocator.
 *
 * to relocate the kernel correctly, we need to look for its base
 * address in the ELF header. this variable must be assigned with this
 * value.
 */

static t_paddr		relocate;

/*
 * the set of page table which have been allocated by the kernel.
 *
 * these page tables must be protected by registering them.
 */

extern t_paddr		pts[PAGING_PTS];
extern t_uint32		npts;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the init structure
 */

void			bootloader_init_dump(void)
{
  t_input*		input;
  t_uint32		i;

  bootloader_cons_msg('#', "dumping memory structure\n");

  bootloader_cons_msg('#', " physical memory: 0x%x - %u bytes\n",
		      init->pbase,
		      init->psize);

  bootloader_cons_msg('#', " kernel code: 0x%x - %u bytes\n",
		      init->kcode,
		      init->kcodesz);

  bootloader_cons_msg('#', " system code: 0x%x - %u bytes :: "
		      "location: 0x%x\n - entry: 0x%x\n",
		      init->scode,
		      init->scodesz,
		      init->slocation,
		      init->sentry);

  bootloader_cons_msg('#', " init structure: 0x%x - %u bytes\n",
		      init,
		      sizeof(t_init));

  bootloader_cons_msg('#', " inputs: 0x%x - %u bytes\n",
		      init->inputs,
		      init->inputssz);

  for (i = 0, input = (t_input*)((t_uint32)init->inputs +
				   sizeof(t_inputs));
       i < init->inputs->ninputs;
       i++)
    {
      bootloader_cons_msg('#', "  [%u] %s 0x%x (0x%x)\n",
			  i,
			  input->name,
			  input,
			  input->size);

      input = (t_input*)((t_uint32)input + sizeof(t_input) +
			   input->size + strlen(input->name) + 1);
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

  bootloader_cons_msg('#', " %#~ia32%# global offset table: 0x%x\n",
		      IBMPC_CONS_FRONT(IBMPC_CONS_CYAN) |
		      IBMPC_CONS_BACK(IBMPC_CONS_BLACK) |
		      IBMPC_CONS_INT,
		      IBMPC_CONS_FRONT(IBMPC_CONS_WHITE) |
		      IBMPC_CONS_BACK(IBMPC_CONS_BLACK) |
		      IBMPC_CONS_INT,
		      init->machine.gdt);

  bootloader_cons_msg('#', " %#~ia32%# page directory: 0x%x\n",
		      IBMPC_CONS_FRONT(IBMPC_CONS_CYAN) |
		      IBMPC_CONS_BACK(IBMPC_CONS_BLACK) |
		      IBMPC_CONS_INT,
		      IBMPC_CONS_FRONT(IBMPC_CONS_WHITE) |
		      IBMPC_CONS_BACK(IBMPC_CONS_BLACK) |
		      IBMPC_CONS_INT,
		      init->machine.pd);
}

/*
 * this function adds the segments to the init variable.
 *
 * steps:
 *
 * 1) add the ISA segment from 0 to 1Mb.
 * 2) add the bootloader segment.
 * 3) add the kernel code segment.
 * 4) add the init structure segment.
 * 5) add the segments segment.
 * 6) add the regions segment.
 * 7) add the cpu segment.
 * 8) add the kernel stack segment.
 * 9) add the alloc segment.
 * 10) add the global offset table segment.
 * 11) add the page directory segment.
 * 12) add the inputs segment.
 * 13) add the system service code segment.
 * 14) add the page tables.
 */

void			bootloader_init_segments(void)
{
  t_uint32		i;

  /*
   * 1)
   */

  init->segments[0].address = INIT_ISA_ADDR;
  init->segments[0].size = PAGESZ;
  init->segments[0].perms = PERM_READ;
  init->segments[0].options = SEGMENT_OPTION_SYSTEM;

  init->segments[1].address = INIT_ISA_ADDR + PAGESZ;
  init->segments[1].size = INIT_ISA_SIZE - PAGESZ;
  init->segments[1].perms = PERM_READ | PERM_WRITE;
  init->segments[1].options = SEGMENT_OPTION_SYSTEM;

  /*
   * 2)
   */

  extern t_uint8 __early_start;
  extern t_uint8 __early_end;
  init->segments[2].address = (t_paddr)&__early_start;
  init->segments[2].size = &__early_end - &__early_start;
  init->segments[2].perms = PERM_READ | PERM_WRITE | PERM_EXEC;
  init->segments[2].options = SEGMENT_OPTION_NONE;

  /*
   * 3)
   */

  init->segments[3].address = init->kcode;
  init->segments[3].size = init->kcodesz;
  init->segments[3].perms = PERM_READ | PERM_WRITE | PERM_EXEC;
  init->segments[3].options = SEGMENT_OPTION_NONE;

  /*
   * 4)
   */

  init->segments[4].address = init->init;
  init->segments[4].size = init->initsz;
  init->segments[4].perms = PERM_READ | PERM_WRITE;
  init->segments[4].options = SEGMENT_OPTION_NONE;

  /*
   * 5)
   */

  init->segments[5].address = (t_paddr)init->segments;
  init->segments[5].size = init->segmentssz;
  init->segments[5].perms = PERM_READ | PERM_WRITE;
  init->segments[5].options = SEGMENT_OPTION_NONE;

  /*
   * 6)
   */

  init->segments[6].address = (t_paddr)init->regions;
  init->segments[6].size = init->regionssz;
  init->segments[6].perms = PERM_READ | PERM_WRITE;
  init->segments[6].options = SEGMENT_OPTION_NONE;

  /*
   * 7)
   */

  init->segments[7].address = (t_paddr)init->cpus;
  init->segments[7].size = init->cpussz;
  init->segments[7].perms = PERM_READ | PERM_WRITE;
  init->segments[7].options = SEGMENT_OPTION_NONE;

  /*
   * 8)
   */

  init->segments[8].address = init->kstack;
  init->segments[8].size = init->kstacksz;
  init->segments[8].perms = PERM_READ | PERM_WRITE;
  init->segments[8].options = SEGMENT_OPTION_NONE;

  /*
   * 9)
   */

  init->segments[9].address = init->alloc;
  init->segments[9].size = init->allocsz;
  init->segments[9].perms = PERM_READ | PERM_WRITE;
  init->segments[9].options = SEGMENT_OPTION_NONE;

  /*
   * 10)
   */

  init->segments[10].address = (t_paddr)init->machine.gdt;
  init->segments[10].size = PAGESZ;
  init->segments[10].perms = PERM_READ | PERM_WRITE;
  init->segments[10].options = SEGMENT_OPTION_SYSTEM;

  /*
   * 11)
   */

  init->segments[11].address = (t_paddr)init->machine.pd;
  init->segments[11].size = PAGESZ;
  init->segments[11].perms = PERM_READ | PERM_WRITE;
  init->segments[11].options = SEGMENT_OPTION_SYSTEM;

  /*
   * 12)
   */

  init->segments[12].address = (t_paddr)init->inputs;
  init->segments[12].size = init->inputssz;
  init->segments[12].perms = PERM_READ | PERM_WRITE | PERM_EXEC;
  init->segments[12].options = SEGMENT_OPTION_NONE;

  /*
   * 13)
   */

  init->segments[13].address = init->scode;
  init->segments[13].size = init->scodesz;
  init->segments[13].perms = PERM_READ | PERM_WRITE | PERM_EXEC;
  init->segments[13].options = SEGMENT_OPTION_NONE;

  /*
   * 14)
   */

  for (i = 0; i < npts; i++)
    {
      init->segments[14 + i].address = pts[i];
      init->segments[14 + i].size = PAGESZ;
      init->segments[14 + i].perms = PERM_READ | PERM_WRITE;
      init->segments[14 + i].options = SEGMENT_OPTION_SYSTEM;
    }

  init->nsegments -= PAGING_PTS - npts;
}

/*
 * this function adds the regions to the init variable.
 *
 * it is important to note that the segid field of the region
 * structures corresponds to the index of the mapped segment into the
 * segments array of the init variable. correct identifiers will be
 * generated by the kernel.
 *
 * steps:
 *
 * 1) add the ISA region.
 * 2) add the bootloader region.
 * 3) add the kernel code region.
 * 4) add the init structure region.
 * 5) add the segments region.
 * 6) add the region region.
 * 7) add the cpu region.
 * 8) add the kernel stack region.
 * 9) add the alloc region.
 * 10) add the global offset table region.
 * 11) add the page directory region.
 * 12) add the page table regions.
 */

void			bootloader_init_regions(void)
{
  t_uint32		i;

  /*
   * 1)
   */

  init->regions[0].segment = 1;
  init->regions[0].address = init->segments[1].address;
  init->regions[0].size = init->segments[1].size;
  init->regions[0].offset = 0;
  init->regions[0].opts = REGION_OPT_FORCE;

  /*
   * 2)
   */

  init->regions[1].segment = 2;
  init->regions[1].address = init->segments[2].address;
  init->regions[1].size = init->segments[2].size;
  init->regions[1].offset = 0;
  init->regions[1].opts = REGION_OPT_FORCE;

  /*
   * 3)
   */

  init->regions[2].segment = 3;
  init->regions[2].address = init->segments[3].address;
  init->regions[2].size = init->segments[3].size;
  init->regions[2].offset = 0;
  init->regions[2].opts = REGION_OPT_FORCE;

  /*
   * 4)
   */

  init->regions[3].segment = 4;
  init->regions[3].address = init->segments[4].address;
  init->regions[3].size = init->segments[4].size;
  init->regions[3].offset = 0;
  init->regions[3].opts = REGION_OPT_FORCE;

  /*
   * 5)
   */

  init->regions[4].segment = 5;
  init->regions[4].address = init->segments[5].address;
  init->regions[4].size = init->segments[5].size;
  init->regions[4].offset = 0;
  init->regions[4].opts = REGION_OPT_FORCE;

  /*
   * 6)
   */

  init->regions[5].segment = 6;
  init->regions[5].address = init->segments[6].address;
  init->regions[5].size = init->segments[6].size;
  init->regions[5].offset = 0;
  init->regions[5].opts = REGION_OPT_FORCE;

  /*
   * 7)
   */

  init->regions[6].segment = 7;
  init->regions[6].address = init->segments[7].address;
  init->regions[6].size = init->segments[7].size;
  init->regions[6].offset = 0;
  init->regions[6].opts = REGION_OPT_FORCE;

  /*
   * 8)
   */

  init->regions[7].segment = 8;
  init->regions[7].address = init->segments[8].address;
  init->regions[7].size = init->segments[8].size;
  init->regions[7].offset = 0;
  init->regions[7].opts = REGION_OPT_FORCE;

  /*
   * 9)
   */

  init->regions[8].segment = 9;
  init->regions[8].address = init->segments[9].address;
  init->regions[8].size = init->segments[9].size;
  init->regions[8].offset = 0;
  init->regions[8].opts = REGION_OPT_FORCE;

  /*
   * 10)
   */

  init->regions[9].segment = 10;
  init->regions[9].address = init->segments[10].address;
  init->regions[9].size = init->segments[10].size;
  init->regions[9].offset = 0;
  init->regions[9].opts = REGION_OPT_FORCE;

  /*
   * 11)
   */

  init->regions[10].segment = 11;
  init->regions[10].address = init->segments[11].address;
  init->regions[10].size = init->segments[11].size;
  init->regions[10].offset = 0;
  init->regions[10].opts = REGION_OPT_FORCE;

  /*
   * 12)
   */

  for (i = 0; i < npts; i++)
    {
      init->regions[11 + i].segment = 14 + i;
      init->regions[11 + i].address = init->segments[14 + i].address;
      init->regions[11 + i].size = PAGESZ;
      init->regions[11 + i].offset = 0;
      init->regions[11 + i].opts = REGION_OPT_FORCE;
    }

  init->nregions -= PAGING_PTS - npts;
}

/*
 * this function just allocates memory for relocation.
 *
 * this function is needed because many objects are dynamic: inputs,
 * page tables etc.
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
 * this function relocates the kernel and inputs over the sixteen megabytes
 * and returns the kernel entry point address.
 *
 * this is not necessary but we wanted to do it properly. moreover this is
 * not entirely useless because the DMA needs the sixteen lowest megabytes.
 *
 * note that the first input will be considered as the kernel input,
 * as multibootloaders do.
 *
 * steps:
 *
 * 1) allocate space for the kernel code. this memory is very special
 *    because it always be located at the address init_relocate.
 *    indeed, the kernel is especially compiled to run at this address.
 * 2) allocate and initialize memory for the init structure
 *    which will be passed to the kernel.
 * 3) compute entire size of the inputs.
 * 4) initialize the main fields of the init structure.
 * 5) copy the kernel code into its new location.
 * 6) initialize the kernel stack.
 * 7) for each input, copy it into its new location.
 * 8) allocate memory for the alloc() function. indeed the alloc() function
 *    needs to be able to provide memory until the virtual memory manager
 *    is installed. thus the alloc() function will be able to ask
 *    the virtual memory manager to provide virtual pages and to map it
 *    into the kernel address space to continue to provide its service.
 *    so the alloc() function needs an amount of critical pages to work with
 *    until the virtual memory is initialized.
 * 9) allocate memory for the system service code.
 */

t_vaddr			bootloader_init_relocate(multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		khdr;
  //Elf32_Phdr*		phdr;
  t_paddr		init_relocate;
  t_uint32		nmods = mbi->mods_count;
  t_uint32		ninputs;
  t_uint32		nsegments = INIT_SEGMENTS + PAGING_PTS;
  t_uint32		nregions = INIT_REGIONS + PAGING_PTS;
  t_psize		inputssz;
  t_psize		segmentssz;
  t_psize		regionssz;
  t_psize		cpussz;
  t_paddr		kcode;
  t_psize		kcodesz;
  t_vaddr		kentry;
  t_psize		allocsz;
  t_input*		input;
  t_psize		initsz;
  t_psize		modsz;
  t_uint32		i;

  bootloader_cons_msg('+', "relocating kernel and inputs...\n");

  /*
   * 1)
   */

  khdr = (Elf32_Ehdr*)mod[0].mod_start;
  //phdr = (Elf32_Phdr*)((char*)khdr + khdr->e_phoff);

  relocate = init_relocate = 0x1000000; // phdr->p_paddr XXX;

  kcode = bootloader_init_alloc(mod[0].mod_end - mod[0].mod_start, &kcodesz);
  kentry = khdr->e_entry;

  if (kcode != init_relocate)
    {
      bootloader_cons_msg('!', "error: kernel relocation impossible\n");
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

  if (nmods == 0)
    {
      bootloader_cons_msg('!', "error: no kernel has been provided\n");
      bootloader_error();
    }

  ninputs = nmods - 1;

  for (inputssz = 0, i = 0; i < ninputs; i++)
    inputssz += mod[1 + i].mod_end - mod[1 + i].mod_start +
      strlen((char*)mod[1 + i].string) + 1;

  /*
   * 4)
   */

  init->pbase = 0x0;
  init->psize = ROUND_SIZE(mbi->mem_upper * 1024);
  init->vbase = 0x0;

  init->vsize = ROUND_SIZE(0x100000000LLU - PAGESZ);
  // XXX this is because this size should
  // be 4GO ie 0x10000000. but this size
  // cannot be held in a 32-bit integer.
  // therefore we remove a page.

  init->init = (t_paddr)init;
  init->initsz = initsz;
  init->kcode = kcode;
  init->kcodesz = kcodesz;

  init->inputs =
    (t_inputs*)bootloader_init_alloc(sizeof(t_inputs) +
				     ninputs * sizeof(t_input) +
				     inputssz,
				     &inputssz);
  memset(init->inputs, 0x0, inputssz);
  init->inputssz = inputssz;
  init->inputs->ninputs = ninputs;

  init->segments =
    (s_segment*)bootloader_init_alloc(nsegments * sizeof(s_segment),
				      &segmentssz);
  memset(init->segments, 0x0, segmentssz);
  init->nsegments = nsegments;
  init->segmentssz = segmentssz;

  init->regions =
    (s_region*)bootloader_init_alloc(nregions * sizeof(s_region),
				     &regionssz);
  memset(init->regions, 0x0, regionssz);
  init->nregions = nregions;
  init->regionssz = regionssz;

  init->cpus = (s_cpu*)bootloader_init_alloc(sizeof(s_cpu), &cpussz);
  memset(init->cpus, 0x0, cpussz);
  init->ncpus = 1;
  init->cpussz = cpussz;

  init->cpus[0].cpuid = 0;
  init->bsp = 0;

  /*
   * 5)
   */

  modsz = mod[0].mod_end - mod[0].mod_start;
  memcpy((void*)init->kcode, (const void*)mod[0].mod_start, modsz);

  bootloader_cons_msg('+', " kernel '%s' relocated from 0x%x to 0x%x (0x%x)\n",
		      mod[0].string, mod[0].mod_start, init->kcode, modsz);

  /*
   * 6)
   */

  init->kstack = bootloader_init_alloc(INIT_KSTACKSZ, NULL);
  init->kstacksz = INIT_KSTACKSZ;

  /*
   * 7)
   */

  for (i = 0,
	 input = (t_input*)((t_uint32)init->inputs + sizeof(t_inputs));
       i < ninputs;
       i++)
    {
      modsz = mod[1 + i].mod_end - mod[1 + i].mod_start;

      input->name = (char*)((t_uint32)input + sizeof(t_input) + modsz);
      strcpy(input->name, (char*)mod[1 + i].string);

      memcpy((void*)((t_uint32)input + sizeof(t_input)),
	     (const void*)mod[1 + i].mod_start, modsz);

      input->size = modsz;

      bootloader_cons_msg('+', " input '%s' relocated from 0x%x to 0x%x (0x%x)\n",
			  input->name,
			  mod[1 + i].mod_start,
			  input,
			  input->size);

      input = (t_input*)((t_uint32)input + sizeof(t_input) +
			 input->size + strlen(input->name) + 1);
    }

  /*
   * 8)
   */

  init->alloc = bootloader_init_alloc(INIT_ALLOCSZ, &allocsz);
  init->allocsz = allocsz;

  /*
   * 9)
   */

  if (ninputs > 0)
    {
      khdr = (Elf32_Ehdr*)mod[1].mod_start;
      //phdr = (Elf32_Phdr*)((char*)khdr + khdr->e_phoff);

      modsz = mod[1].mod_end - mod[1].mod_start;

      init->scode = bootloader_init_alloc(modsz, &init->scodesz);
      init->slocation = 0x60000000; /* XXX very ugly hard-coded value */
      init->sentry = khdr->e_entry;

      memcpy((void*)init->scode, (const void*)mod[1].mod_start, modsz);

      bootloader_cons_msg('+', " server '%s' relocated from 0x%x to 0x%x (0x%x)\n",
			  mod[1].string, mod[1].mod_start, init->scode, modsz);
    }
  else
    {
      init->scode = 0;
      init->slocation = 0;
      init->sentry = 0;
    }

  return (kentry);
}
