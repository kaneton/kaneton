/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/machdep/init.c
 *
 * created       enguerrand raymond   [sun nov 19 16:19:55 2006]
 * updated       enguerrand raymond   [sun jan 14 10:25:00 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <libmipsr10000.h>
#include <kaneton.h>

#include "bootloader.h"
#include "init.h"

/*
 * ---------- globals -------------------------------------------------------
 */

/*
 * init variable, describing the initial memory layout.
 *
 * this structure will be passed to the kernel describing modules,
 * physical memory reserved by the hardware etc. thus the kernel will
 * be able to initialise its managers: segment manager, module manager etc.
 */

t_init*		init;

t_uint32	addr_alloc = KERNEL_BASE32 + KERNEL_SIZE;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * Function to allocate memory by page of PAGESIZE
 */

t_uint32	bootloader_alloc(t_uint64	size)
{
  t_uint32	res = addr_alloc;

  addr_alloc += nbr_page(size) * 4096;
  return res;
}


/*
 * Function to perform memory size using firmware
 */

t_uint32		bootloader_memory_size(void)
{
  MEMORYDESCRIPTOR*	mem_desc = NULL;
  t_uint32		page_count = 0;

  while ((mem_desc = ArcGetMemoryDescriptor(mem_desc)) != NULL)
    page_count += mem_desc->PageCount;
  return (page_count * 4) / 1024;
}

/*
 * This function add all kernel segments in the init structure
 *
 * 1) Null segment, the first of memory
 * 2) Kernel segment
 * 3) t_init stucture segment
 * 4) Array of kernel modules
 * 5) Array of kernel segments
 * 6) Kernel stack segment
 * 7) Kernel malloc zone segment
 */

void	bootloader_segments_init(void)
{
  /*
   * 1)
   */

  init->segments[0].address = 0;
  init->segments[0].size = PAGESZ;
  init->segments[0].perms = 0;
  init->segments[0].type = SEGMENT_TYPE_MEMORY;

  /*
   * 2)
   */

  init->segments[1].address = init->kcode;
  init->segments[1].size = init->kcodesz;
  init->segments[1].perms = PERM_READ | PERM_WRITE | PERM_EXEC;
  init->segments[1].type = SEGMENT_TYPE_MEMORY;

  /*
   * 3)
   */

  init->segments[2].address = init->init;
  init->segments[2].size = init->initsz;
  init->segments[2].perms = PERM_READ | PERM_WRITE;
  init->segments[2].type = SEGMENT_TYPE_MEMORY;

  /*
   * 4)
   */

  init->segments[3].address = (t_paddr)init->modules;
  init->segments[3].size = init->modulessz;
  init->segments[3].perms = PERM_READ | PERM_WRITE;
  init->segments[3].type = SEGMENT_TYPE_MEMORY;

  /*
   * 5)
   */

  init->segments[4].address = (t_paddr)init->segments;
  init->segments[4].size = init->segmentssz;
  init->segments[4].perms = PERM_READ | PERM_WRITE;
  init->segments[4].type = SEGMENT_TYPE_MEMORY;

  /*
   * 6)
   */

  init->segments[5].address = init->kstack;
  init->segments[5].size = init->kstacksz;
  init->segments[5].perms = PERM_READ | PERM_WRITE;
  init->segments[5].type = SEGMENT_TYPE_MEMORY;

  /*
   * 7)
   */

  init->segments[6].address = init->alloc;
  init->segments[6].size = init->allocsz;
  init->segments[6].perms = PERM_READ | PERM_WRITE;
  init->segments[6].type = SEGMENT_TYPE_MEMORY;
}

/*
 * This function fills init structure fields
 *
 * 1) init memory RAM size
 * 2) init kernel base and size
 * 3) fill the modules field to fill modules address
 * 4) fill the segment array address
 * 5) It is useless to fill region because kernel
 *    regions do not need to be mapped because kernel regions
 *    are located in unmapped physical space
 * 6) fill the new stack address
 * 7) fill the alloc address for kernel malloc
 */

void			bootloader_init(void)
{
  /*
   * 1)
   */

  init->mem = START_PHYSICAL_ADDR;
  init->memsz = bootloader_memory_size();

  /*
   * 2)
   */

  init->kcode = (t_paddr)KERNEL_BASE32;
  init->kcodesz = (t_psize)(nbr_page(KERNEL_SIZE) * PAGESZ);

  /*
   * 3)
   */

  init->modules = (t_modules*)bootloader_alloc(0);
  init->modulessz = 0;

  /*
   * 4)
   */

  init->nsegments = 7;
  init->segments = (o_segment*)bootloader_alloc(PAGESZ);

  /*
   * 5)
   */

  init->nregions = (t_modules*)bootloader_alloc(0);
  init->regions = 0;

  /*
   * 6)
   */

  init->kstack = (t_paddr)bootloader_alloc(KSTACK_SIZE);
  init->kstacksz = (t_psize)KSTACK_SIZE;

  /*
   * 7)
   */

  init->alloc = (t_paddr)bootloader_alloc(ALLOC_SIZE);
  init->allocsz = (t_psize)ALLOC_SIZE;
}
