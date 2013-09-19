/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/serial.c
 *
 * created       julio guerra   [mon sept  15 13:45:13 2013]
 * updated       julio guerra   [web sept  18 22:59:10 2013]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements wrapper functions calling the actual platform serial
 * driver
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "serial.h"

/*
 * ---------- functions -------------------------------------------------------
 */

void dbg_serial_read(t_uint8* buf, t_uint32 sz)
{
  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
                       buf,
                       sz);
}

void dbg_serial_write(t_uint8* buf, t_uint32 sz)
{
  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
                        buf,
                        sz);
}
