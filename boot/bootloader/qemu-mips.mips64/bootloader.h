/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...bootloader/qemu-mips.mips64/bootloader.h
 *
 * created       enguerrand raymond   [sun oct 12 18:58:49 2008]
 * updated          [sun apr  5 06:05:43 2009]
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
#include "libmips64.h"

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

t_paddr		bootloader_init_alloc(t_psize	size,
				      t_psize*	psize);

t_size		bootloader_init_kernel_size(t_vaddr	kernel_address);

t_init*		bootloader_init_relocate(t_vaddr	kernel_address);


/*
 * eop
 */

#endif
