/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...t/bootloader/qemu-mips.mips64/firmware.c
 *
 * created       enguerrand raymond   [fri apr 10 13:11:23 2009]
 * updated       enguerrand raymond   [sun apr 12 01:02:29 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "firmware.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

extern t_uint32*	flag_address;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * a funny function which does nothing.
 *
 * this function is called when a fatal error occurs.
 */

void			firmware_error(void)
{
  while (1)
    ;
}

void			firmware(void)
{
  t_vaddr		image = 0x0;
  multiboot_info_t*	mbi = (multiboot_info_t*)MULTIBOOT_ADDRESS;

  if(*flag_address != FIRMWARE_FLAG)
    firmware_error();

  image = (t_vaddr)(flag_address + 1);

  

  while(1);
}
