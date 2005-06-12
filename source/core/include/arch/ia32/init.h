/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * init.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Mon May 30 14:10:03 2005   mycure
 * last update   Fri Jun 10 16:44:49 2005   mycure
 */

#ifndef IA32_INIT_H
#define IA32_INIT_H		1

/*
 * this address defines the 16Mb. the kernel will be relocates at this
 * address followed by many data structures: modules, segments, regions,
 * global offset tables, page directory etc..
 */

#define INIT_RELOCATE		0x1000000

/*
 * the segments represent the physical memory areas to keep safe.
 *
 * the segments are composed of ISA, kernel code, the init structure, the
 * module structures, the segment structures, the region structures, the
 * kernel stack, the segment manager structure, the global offset table and
 * the page directory.
 */

#define INIT_SEGMENTS		10

/*
 * the regions represent the physical memory to be mapped by the kernel.
 *
 * indeed, some segments do not need to be accessed by the kernel after
 * the kernel initialization like the module structures, the segment
 * structures and the region structures.
 *
 * moreover, the kernel will rebuild a page directory so the page tables
 * are now needless explaining why they are neither present in the segments
 * nor in the regions.
 */

#define INIT_REGIONS		7

#endif
