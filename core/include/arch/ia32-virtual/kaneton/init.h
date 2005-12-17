/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/arch/ia32-virtual/kaneton/init.h
 *
 * created       julien quintard   [sat dec 17 17:15:29 2005]
 * updated       julien quintard   [sat dec 17 17:15:32 2005]
 */

#ifndef IA32_KANETON_INIT_H
#define IA32_KANETON_INIT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/ia32/types.h>
#include <arch/machdep/ia32/pmode.h>
#include <arch/machdep/ia32/paging.h>

#include <arch/machdep/services/cons.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * this address defines the 16Mb. the kernel will be relocates at this
 * address followed by many data structures: modules, segments, regions,
 * global offset tables, page directory etc..
 */

#define INIT_RELOCATE		0x1000000

/*
 * the segments represent the physical memory areas to keep safe.
 * once the kernel will be launched, it will initialise its segment manager,
 * in other words, its physical memory manager. the segments listed in the
 * init structure specify the areas to reserve because they are to be
 * never written.
 *
 * the segments are composed of ISA, kernel code, the init structure, the
 * module structures, the segment structures, the region structures, the
 * kernel stack, the malloc preallocated pages, the global offset table
 * and the page directory.
 */

#define INIT_SEGMENTS		11

/*
 * the regions represent the physical memory to be mapped by the kernel.
 * once the kernel will have initialised its segment manager, it will
 * initialise its region manager, in other words, its virtual memory manager.
 * the regions specify the segments to map after the initialisation process.
 *
 * indeed, some segments do not need to be accessed by the kernel after
 * the kernel initialisation like the module structures, the segment
 * structures and the region structures.
 *
 * moreover, the kernel will rebuild a page directory so the page tables
 * are now needless explaining why they are neither present in the segments
 * nor in the regions.
 */

#define INIT_REGIONS		7

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_gdtr			gdtr;
  t_pde*			pd;

  t_cons			cons; /* XXX */
}				d_init;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * machdep traps
 */

#define		machdep_data_init()					\
  d_init			machdep

#endif
