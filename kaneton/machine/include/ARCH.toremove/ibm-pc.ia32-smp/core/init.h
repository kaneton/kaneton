/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ibm-pc.ia32-smp/core/init.h
 *
 * created       julien quintard   [sat dec 17 17:15:29 2005]
 * updated       matthieu bucchianeri   [tue mar 13 11:53:10 2007]
 */

#ifndef IA32_CORE_INIT_H
#define IA32_CORE_INIT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libia32.h>
#include <arch/machdep/services/cons.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

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

#define INIT_SEGMENTS		14

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

#define INIT_REGIONS		12

/*
 * the kernel stack size is set to two pages.
 */

#define INIT_KSTACKSZ		(2 * PAGESZ)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_ia32_gdt			gdt;
  t_ia32_directory		pd;
  t_ia32_idt			idt;
}				a_init;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * machdep traps
 */

#define		machine_data_init()					\
  a_init			machdep

#endif
