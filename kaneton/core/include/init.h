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
 * updated       julien quintard   [fri jan 14 22:43:03 2011]
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
  t_options			options;
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
 * the 'pbase' and 'psize' represent the base and size of the underlying
 * physical memory layout.
 *
 * the 'kcode' and 'kcodesz' represent the location of the kernel code
 * in the physical memory along with its size.
 *
 * the 'scode' and 'scodesz' represent the location of the very first
 * server to spawn, the 'system' service, in the physical memory along with
 * its size. in addition, the 'slocation' indicates the address in the
 * virtual memory where the server's code should be mapped. finally, the
 * 'sentry' indicates the virtual address of the entry point for the server's
 * code.
 *
 * the 'init' and 'initsz' represent the physical location and size of the
 * whole init structure. indeed, the init structure is passed to the kernel
 * in as a package i.e packed in a single area of memory. this meory area
 * must be taken care of in order to prevent it to be erased for instance.
 *
 * the 'inputs' and 'inputssz' represent the physical location and size
 * of the array containing the inputs i.e the servers to be spawn at
 * the startup by the _system_ server.
 *
 * the 'nsegments' indicates the number of segments registered in the
 * array defined by 'segments' and 'segmentssz'. the segment registered
 * in this array must be considered by the kernel as being allocated.
 * therefore, the kernel should add these segments to its own data structures
 * as soon as its memory managers have been initialized. in more practical
 * terms, such segments may reference memory mapped devices such as the
 * DMA - Direct Memory Access memory area, the kernel code area, the
 * init structure area, the inputs array, the array of segments itself,
 * the kernel stack, and so on and so forth.
 *
 * the 'nregions', 'regions' and 'regionssz' provide the same kind of
 * array but for pre-reserved regions. such regions include the kernel
 * code and stack which must obviously be mapped etc.
 *
 * the 'ncpus', 'cpus' and 'cpussz' indicate an array containing the
 * descriptions of the CPUs present on the system i.e for multi-processor
 * systems. on a mono-processor system, the array is obviously composed
 * of a single entry i.e _ncpus_ equals one. finally, the 'bsp' indicates
 * the identifier of CPU acting as the BSP - Boot Strap Processor i.e
 * the initally running processor. note that the 'bsp' does not provides
 * an index in the array of CPUs but the kaneton identifier of the BSP.
 *
 * the 'kstack' and 'kstacksz' indicate the physical location and size of the
 * kernel stack.
 *
 * finally, the 'alloc' and 'allocsz' indicate the virtual location of
 * a memory area to be used by the kaneton fine-grained memory allocator
 * until the kernel's memory manager have been set up. indeed, the kaneton
 * design relies on sets for managing data structures. the memory managers
 * such as the segment, region and as managers rely on the set manager for
 * storing their information. therefore, the memory manager rely on another
 * manager for storing their data while the set manager relies on malloc()
 * for allocating memory. and obviously, malloc() relies on the memory
 * managers for retrieving additional memory when it runs out of space.
 * this infinite loop can be avoided by providing malloc() a temporary
 * zone of allocation i.e the pre-allocated memory area referenced by
 * 'alloc' and 'allocsz'.
 *
 * note that the kernel expects to receive a sorted array of segments and
 * regions, the segment[0] and region[0] being the lowest in memory.
 *
 * besides, note that the size fields represent sizes aligned on
 * ___kaneton$pagesz.
 */

typedef struct
{
  t_paddr			pbase;
  t_psize			psize;

  t_paddr			vbase;
  t_psize			vsize;

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
