/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...t/bootloader/qemu-mips.mips64/firmware.h
 *
 * created       enguerrand raymond   [fri apr 10 13:11:49 2009]
 * updated       enguerrand raymond   [fri apr 17 20:44:50 2009]
 */

#ifndef FIRMWARE_H
#define FIRMWARE_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"
#include "libc.h"
#include "multiboot.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define FIRMWARE_FLAG		0x07070707

#define IMAGE_BASE_ADDRESS	0xffffffffa0100000

#define MULTIBOOT_ADDRESS	0xffffffffa0300000

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint32	modsz;
  char*		name;
}	kaneton_mips_module;

typedef struct
{
  t_uint32	nmod;
}	header;

/*
 * ---------- prototypes ------------------------------------------------------
 * 
 *	firmware.c
 */

/*
 * firmware.c
 */

void			firmware_error(void);

void			firmware(void);


/*
 * eop
 */

#endif
