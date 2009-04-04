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
 * updated          [fri apr  3 11:57:59 2009]
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

#include "types.h"
#include "libc.h"
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

void		bootloader_error(void);

void		bootloader(void);


/*
 * init.c
 */

t_size		kernel_size(t_vaddr	kernel_flag);

t_vaddr		bootloader_kernel_init(t_vaddr kernel_address);


/*
 * eop
 */

#endif
