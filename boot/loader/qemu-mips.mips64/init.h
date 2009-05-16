/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/kaneton/boot/loader/qemu-mips.mips64/init.h
 *
 * created       matthieu bucchianeri   [wed jul 18 18:10:19 2007]
 * updated       enguerrand raymond   [wed may 13 08:39:12 2009]
 */

#ifndef BOOTLOADER_INIT_H
#define BOOTLOADER_INIT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the kernel is moved in the ckseg0 region
 */

#define KERNEL_BASE_ADDRESS	0xffffffff80001000

/*
 * 4K page size is equal to 0 for the page mask
 * register
 */

#define PAGESZ			0x1000
#define PAGE_SIZE		0

/*
 * Kernel layout size
 */

#define STACKSZ			0x2000
#define ALLOCSZ			0x10000

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * types for compatibility
 */

typedef t_uint32	t_perms;
typedef t_uint32	t_opts;

/*
 * input
 */

typedef struct
{
  char*				name;
  t_psize			size;
}				t_input;

/*
 * inputs
 */

typedef struct
{
  t_uint32			ninputs;
}				t_inputs;

/*
 * segment structure description
 */

typedef struct
{
  t_paddr			address;
  t_psize			size;
  t_perms			perms;
}				s_segment;

/*
 * region structure description
 *
 * the 'segment' field represents an index in the array of segments.
 */

typedef struct
{
  t_uint32			segment;

  t_vaddr			address;
  t_paddr			offset;
  t_vsize			size;
  t_opts			opts;
}				s_region;

/*
 * cpu structure description
 */

typedef struct
{
  t_id				cpuid;
}				s_cpu;

/*
 * the init structure
 *
 * note that concerning the segments and the regions, the kernel waits
 * for sorted segments and regions the segment[0] being the lowest in
 * memory.
 *
 * note that the size fields represent sizes aligned on PAGESZ.
 */

typedef struct
{
  t_paddr			mem;
  t_psize			memsz;

  t_paddr			kcode;
  t_psize			kcodesz;

  t_paddr			mcode;
  t_psize			mcodesz;
  t_vaddr			mlocation;
  t_vaddr			mentry;

  t_paddr			init;
  t_psize			initsz;

  t_inputs*			inputs;
  t_psize			inputssz;

  t_uint32			nsegments;
  s_segment*			segments;
  t_psize			segmentssz;

  t_uint32			nregions;
  s_region*			regions;
  t_psize			regionssz;

  t_uint32			ncpus;
  s_cpu*			cpus;
  t_psize			cpussz;
  t_id				bsp;

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			alloc;
  t_psize			allocsz;

  struct
  {
  }				machine;
}				t_init;

#endif
