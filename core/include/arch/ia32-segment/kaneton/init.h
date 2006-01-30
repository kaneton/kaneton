/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/export_kaneton/kaneton/core/include/arch/ia32-segment/kaneton/init.h
 *
 * created       julien quintard   [sat dec 17 17:15:29 2005]
 * updated       matthieu bucchianeri   [mon jan 30 20:39:56 2006]
 */

#ifndef IA32_KANETON_INIT_H
#define IA32_KANETON_INIT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libia32.h>
#include <arch/machdep/services/cons.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * this address defines the 16Mb. the kernel will be relocates at this
 * address followed by many data structures: modules, segments, regions,
 * global offset tables etc..
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
 * kernel stack, the malloc preallocated area and the global offset table
 */

#define INIT_SEGMENTS		10

/*
 * the regions represent the physical memory to be mapped by the kernel.
 * once the kernel will have initialised its segment manager, it will
 * initialise its region manager, in other words, its virtual memory manager.
 * the regions specify the segments to map after the initialisation process.
 *
 * indeed, some segments do not need to be accessed by the kernel after
 * the kernel initialisation like the module structures, the segment
 * structures and the region structures.
 */

#define INIT_REGIONS		7

/*
 * the kernel stack size is set to 8 kilo-bytes.
 */

#define INIT_KSTACKSZ		8192

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
/*                                                                [cut] k1   */

  t_gdt				gdt;
  t_directory			pd;

/*                                                               [cut] /k1   */

  t_cons			cons;
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
