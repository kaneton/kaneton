/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/libs/libmipsr10000/include/o2.h
 *
 * created       enguerrand raymond   [wed nov 15 18:19:32 2006]
 * updated       enguerrand raymond   [sun jan  7 16:35:44 2007]
 */

#ifndef O2_H
#define O2_H		1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define SPB_ADDR		0xA0001000UL
#define PAGESIZE		0x1000
#define START_PHYSICAL_ADDR	0x0
#define KERNEL_BASE64		0xffffffff8000f000
#define KERNEL_BASE32		0x8000f000
#define KSTACK_SIZE		0xa000
#define ALLOC_SIZE		0x400000

#endif
