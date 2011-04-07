/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ot/loader/ibm-pc.ia32/educational/init.h
 *
 * created       matthieu bucchianeri   [wed jul 18 18:10:19 2007]
 * updated       julien quintard   [thu apr  7 14:55:29 2011]
 */

#ifndef BOOTLOADER_INIT_H
#define BOOTLOADER_INIT_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"
#include "libia32.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define INIT_SEGMENTS		14
#define INIT_REGIONS		11
#define INIT_KSTACKSZ		(4 * PAGESZ)
#define INIT_ALLOCSZ		(256 * PAGESZ)

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
  t_options			options;
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
  t_paddr			pbase;
  t_psize			psize;

  t_vaddr			vbase;
  t_vsize			vsize;

  t_paddr			kcode;
  t_psize			kcodesz;

  t_paddr			scode;
  t_psize			scodesz;
  t_vaddr			slocation;
  t_vaddr			sentry;

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
    t_paddr			gdt;
    t_paddr			pd;
  }				machine;
}				t_init;

#endif
