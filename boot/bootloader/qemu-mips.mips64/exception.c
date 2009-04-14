/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/.../bootloader/qemu-mips.mips64/exception.c
 *
 * created          [wed apr  8 08:04:50 2009]
 * updated          [wed apr  8 08:13:03 2009]
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "libmips64.h"

/*
 * ---------- functions -------------------------------------------------------
 */

void	cache_error(void)
{
  while(1)
    ;
}

void	general_exception(void)
{
  while(1)
    ;
}

void	xtlb_refill(void)
{
  while(1)
    ;
}
