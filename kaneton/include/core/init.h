/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/init.h
 *
 * created       julien quintard   [wed jun  6 13:20:24 2007]
 * updated       julien quintard   [wed jun  6 20:01:52 2007]
 */

#ifndef GUARD_CORE_INIT
#define GUARD_CORE_INIT			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/core.h>

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

/* XXX temporary, hopefully */
#include <arch/machdep/machdep.h>

typedef struct
{
  t_paddr			address;
  t_psize			size;
  t_perms			perms;
}				s_segment;

typedef struct
{
  t_paddr			physical;
  t_vaddr			virtual;
  t_vaddr			offset;
  t_vsize			size;
  t_opts			opts;
}				s_region;
/* /XXX */

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
  s_segment*			segments;
  t_psize			segmentssz;

  t_uint32			nregions;
  s_region*			regions;
  t_psize			regionssz;

  /* XXX
  t_uint32			ncpus;
  s_cpu*			cpus;
  t_psize			cpussz;
  i_cpu				bsp;
  */

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			alloc;
  t_psize			allocsz;

  machdep_data(init);
};

#endif
