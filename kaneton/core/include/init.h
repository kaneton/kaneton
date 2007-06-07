/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/init.h
 *
 * created       julien quintard   [wed jun  6 13:20:24 2007]
 * updated       julien quintard   [thu jun  7 16:04:13 2007]
 */

/*
 * XXX
 *
 * this file must be reworked as the t_init structure directly uses object
 * definitions. this is very bad!
 *
 * instead, specific basic definitions should be used: s_segment, s_region etc.
 * with less fields. for example, the bootloader ony fill the address, size,
 * and perms fields of the segment object. so why not create a s_segment
 * structure with these three fields.
 *
 * to avoid any problem at the moment, this file must be included by core.h
 * in the very last to be sure the object are defined: o_segment, o_region
 * and o_cpu.
 */

#ifndef CORE_INIT_H
#define CORE_INIT_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

/* XXX */
#include <core/segment.h>
#include <core/region.h>
#include <core/cpu.h>

#include <machine/machine.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * modules
 */

typedef struct
{
  char*				name;
  t_psize			size;
}				t_module;

typedef struct
{
  t_uint32			nmodules;
}				t_modules;

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

  t_paddr			init;
  t_psize			initsz;

  t_modules*			modules;
  t_psize			modulessz;

  t_uint32			nsegments;
  o_segment*			segments;
  t_psize			segmentssz;

  t_uint32			nregions;
  o_region*			regions;
  t_psize			regionssz;

  t_uint32			ncpus;
  o_cpu*			cpus;
  t_psize			cpussz;
  i_cpu				bsp;

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			alloc;
  t_psize			allocsz;

  machdep_data(init);
}				t_init;

#endif
