/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/export_kaneton/kaneton/core/include/kaneton/init.h
 *
 * created       julien quintard   [fri feb 11 02:19:11 2005]
 * updated       matthieu bucchianeri   [mon jan 30 20:40:11 2006]
 */

#ifndef KANETON_INIT_H
#define KANETON_INIT_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/segment.h>
#include <kaneton/region.h>

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

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			alloc;
  t_psize			allocsz;

  machdep_data(init);
}				t_init;

#endif
