/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/include/init.h
 *
 * created       julien quintard   [wed jun  6 13:20:24 2007]
 * updated       julien quintard   [mon nov 22 11:31:42 2010]
 */

#ifndef CORE_INIT_H
#define CORE_INIT_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

/*
 * ---------- types -----------------------------------------------------------
 */

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
  t_permissions			permissions;
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
  t_options			options;
}				s_region;

/*
 * cpu structure description
 */

typedef struct
{
  i_cpu				cpu;
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
  i_cpu				bsp;

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			alloc;
  t_psize			allocsz;

  machine_data(init);
}				t_init;

#endif
