/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...oot/loader/qemu-mips.mips64/bootloader.h
 *
 * created       enguerrand raymond   [sun oct 12 18:58:49 2008]
 * updated       enguerrand raymond   [wed may 13 08:38:47 2009]
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H	1

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
#include "elf.h"
#include "multiboot.h"
#include "cons.h"

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

void		bootloader(multiboot_info_t* mbi);


/*
 * init.c
 */

t_paddr		bootloader_init_alloc(t_paddr*	relocate,
				      t_psize	size,
				      t_psize*	psize);

t_init*		bootloader_init_relocate(multiboot_info_t*	mbi);


/*
 * eop
 */

#endif
