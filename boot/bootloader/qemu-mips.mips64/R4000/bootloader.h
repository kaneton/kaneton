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
 * updated          [sun feb  8 15:39:58 2009]
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
 * ---------- prototypes ------------------------------------------------------
 *
 *      bootloader.c
 */

/*
 * bootloader.c
 */

void	bootloader(void);


/*
 * eop
 */

#endif
