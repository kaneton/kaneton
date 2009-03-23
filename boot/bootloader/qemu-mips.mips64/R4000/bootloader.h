/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...ader/qemu-mips.mips64/R4000/bootloader.h
 *
 * created       enguerrand raymond   [sun oct 12 18:58:49 2008]
 * updated          [mon mar 23 08:02:25 2009]
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#define		machine_data(_object_)					\
  //machine_data_##_object_()

#define		machine_data_init()					\
  struct								\
  {									\
  }

/*
 * ---------- includes --------------------------------------------------------
 */

#include "cp0.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define KERNEL_BASE_ADDRESS	0xffffffff80000000UL

/*
 * 4K page size is equal to 0 for the page mask
 * register
 */

#define PAGE_SIZE	0


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	bootloader.c
 */

/*
 * bootloader.c
 */

void		bootloader_error(void);

void		kernel_move(char* kernel_src, char* kernel_dest);

void		bootloader(void);


/*
 * eop
 */

#endif
