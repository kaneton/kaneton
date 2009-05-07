/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...ue/qemu-mips.mips64/R4000/include/init.h
 *
 * created       enguerrand raymond   [sun oct 12 19:02:43 2008]
 * updated       enguerrand raymond   [sun jan 25 15:18:07 2009]
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

#define INIT_SEGMENTS		13

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * machine traps
 */

#define		machine_data_init()					\
  struct								\
  {									\
  }			machine;

#endif
