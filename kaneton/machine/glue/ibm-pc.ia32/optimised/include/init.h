/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...lue/ibm-pc.ia32/optimised/include/init.h
 *
 * created       julien quintard   [wed jun  6 16:16:48 2007]
 * updated       matthieu bucchianeri   [mon dec 24 18:42:15 2007]
 */

#ifndef GLUE_INIT_H
#define GLUE_INIT_H		1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the segments represent the physical memory areas to keep safe.
 * once the kernel will be launched, it will initialize its segment manager,
 * in other words, its physical memory manager. the segments listed in the
 * init structure specify the areas to reserve because they are to be
 * never written.
 *
 * the segments are composed of ISA, kernel code, the init structure, the
 * module structures, the segment structures, the region structures, the
 * kernel stack, the malloc preallocated pages, the mod service code, the
 * global offset table and the page directory.
 */

#define INIT_SEGMENTS		14

/*
 * the regions represent the physical memory to be mapped by the kernel.
 * once the kernel will have initialized its segment manager, it will
 * initialize its region manager, in other words, its virtual memory manager.
 * the regions specify the segments to map after the initialization process.
 *
 * indeed, some segments do not need to be accessed by the kernel after
 * the kernel initialization like the module structures, the segment
 * structures and the region structures.
 *
 * moreover, the kernel will rebuild a page directory so the page tables
 * are now needless explaining why they are neither present in the segments
 * nor in the regions.
 */

#define INIT_REGIONS		11

/*
 * the kernel stack size is set to two pages.
 */

#define INIT_KSTACKSZ		(4 * PAGESZ)

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * machine traps
 */

#define		machine_data_init()					\
  struct								\
  {									\
    t_ia32_gdt			gdt;					\
    t_ia32_idt			idt;					\
    t_ia32_directory		pd;					\
  }				machine;

#endif
