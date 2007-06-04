/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/init.h
 *
 * created       julien quintard   [fri feb 11 02:19:11 2005]
 * updated       julien quintard   [thu aug 31 20:13:57 2006]
 */

#ifndef CORE_INIT_H
#define CORE_INIT_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct s_module		t_module;
typedef struct s_modules	t_modules;
typedef struct s_init		t_init;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/segment.h>
#include <core/region.h>
#include <core/cpu.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * modules
 */

struct				s_module
{
  char*				name;
  t_psize			size;
};

struct				s_modules
{
  t_uint32			nmodules;
};

/*
 * the init structure
 *
 * note that concerning the segments and the regions, the kernel waits
 * for sorted segments and regions the segment[0] being the lowest in
 * memory.
 *
 * note that the size fields represent sizes aligned on PAGESZ.
 */

struct				s_init
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
};

#endif
