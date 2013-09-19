/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/serial.h
 *
 * created       julio guerra   [mon sept  15 13:49:44 2013]
 * updated       julio guerra   [web sept  18 21:57:24 2013]
 */

#ifndef MODULES_DEBUG_SERIAL_H
# define MODULES_DEBUG_SERIAL_H 1

/*
 * ---------- includes --------------------------------------------------------
 */

# include "kaneton.h"

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * serial.c
 */

void dbg_serial_read(t_uint8* buf, t_uint32 sz);

void dbg_serial_write(t_uint8* buf, t_uint32 sz);

#endif /* !MODULES_DEBUG_SERIAL_H */
