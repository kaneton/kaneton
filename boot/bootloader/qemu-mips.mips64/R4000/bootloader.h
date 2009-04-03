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
 * updated          [thu apr  2 14:41:12 2009]
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
 * ---------- dependencies ----------------------------------------------------
 */

#include "init.h"
#include "libR4000.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	bootloader.c
 *	init.c
 */

/*
 * bootloader.c
 */

void		kernel_move(t_uint8* kernel_src, t_uint8* kernel_dest);

void		bootloader_error(void);

void		bootloader(void);

/*
 * init.c
 */

/*
 * eop
 */

#endif
