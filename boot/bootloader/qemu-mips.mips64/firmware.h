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
 * updated       enguerrand raymond   [sun apr 12 01:02:05 2009]
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
 * ---------- prototypes ------------------------------------------------------
 * 
 *	firmware.c
 */

/*
 * firmware.c
 */

void	firmware_error(void);

void	firmware(void);


/*
 * eop
 */

#endif
