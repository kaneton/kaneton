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
 * updated          [sun mar 22 19:36:30 2009]
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

#define KERNEL_BASE_ADDRESS	0xb000000000000000U

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	bootloader.c
 */

/*
 * bootloader.c
 */

void			bootloader_error(void);

void			bootloader(void);


/*
 * eop
 */

#endif
