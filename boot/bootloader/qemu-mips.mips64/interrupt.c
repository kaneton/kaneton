/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/.../bootloader/qemu-mips.mips64/interrupt.c
 *
 * created       enguerrand raymond   [mon mar 23 17:50:38 2009]
 * updated       enguerrand raymond   [sat apr 11 02:38:22 2009]
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "libmips64.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * clear an IP bits of the cause register
 */

void	clear_ip_bit(t_uint32 value)
{
  clear_cause_register_bit(value);
}

/*
 * clear all cause register. Use for stating
 */

void	clear_cause_register(void)
{
  clear_cause_register_bit(0);
}
