/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...ool/firmware/qemu-mips.mips64/firmware.h
 *
 * created       enguerrand raymond   [fri apr 10 13:11:49 2009]
 * updated       enguerrand raymond   [wed may 13 08:42:47 2009]
 */

#ifndef FIRMWARE_H
#define FIRMWARE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"
#include "libc.h"
#include "multiboot.h"
#include "cons.h"
#include "elf.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define FIRMWARE_FLAG		0x07070707

#define IMAGE_BASE_ADDRESS	0xffffffffa0400000

#define MULTIBOOT_ADDRESS	0xffffffffa0800008

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint64	modsz;
  char*		name;
  t_vaddr	module;
}	kaneton_mips_module;

typedef struct
{
  t_uint32	nmod;
}	kaneton_mips_header;

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
