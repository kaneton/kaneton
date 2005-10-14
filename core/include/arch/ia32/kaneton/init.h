/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * init.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Mon May 30 14:10:03 2005   mycure
 * last update   Fri Oct 14 10:34:36 2005   mycure
 */

#ifndef IA32_KANETON_INIT_H
#define IA32_KANETON_INIT_H	1

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

#endif
