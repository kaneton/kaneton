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
 * updated       julien quintard   [fri dec 10 11:35:32 2010]
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
 * the structure for an input i.e a server that must be launched at startup.
 */

typedef struct
{
  char*				name;
  t_psize			size;
}				s_init_input;

/*
 * the header of the inputs array containing the number of entries.
 */

typedef struct
{
  t_uint32			ninputs;
}				s_init_inputs;

/*
 * the segment structure description.
 */

typedef struct
{
  t_paddr			address;
  t_psize			size;
  t_permissions			permissions;
}				s_init_segment;

/*
 * the region structure description.
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
}				s_init_region;

/*
 * the CPU structure description.
 */

typedef struct
{
  i_cpu				cpu;
}				s_init_cpu;

/*
 * the init structure.
 *
 * note that the kernel expects to receive a sorted array of segments and
 * regions, the segment[0] and region[0] being the lowest in memory.
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

  s_init_inputs*		inputs;
  t_psize			inputssz;

  t_uint32			nsegments;
  s_init_segment*		segments;
  t_psize			segmentssz;

  t_uint32			nregions;
  s_init_region*		regions;
  t_psize			regionssz;

  t_uint32			ncpus;
  s_init_cpu*			cpus;
  t_psize			cpussz;
  i_cpu				bsp;

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			alloc;
  t_psize			allocsz;

  machine_data(s_init);
}				s_init;

#endif
