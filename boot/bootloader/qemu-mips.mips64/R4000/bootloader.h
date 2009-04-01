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
 * updated          [wed apr  1 09:45:30 2009]
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

#include "r4000.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define KERNEL_BASE_ADDRESS	0xffffffff80020000

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

void		kernel_move(char* kernel_src, char* kernel_dest);

void		bootloader_error(void);

void		bootloader(void);


/*
 * eop
 */

#endif
