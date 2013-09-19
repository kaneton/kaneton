/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/misc.h
 *
 * created       julio guerra   [mon sept  15 13:49:44 2013]
 * updated       julio guerra   [web sept  18 21:48:17 2013]
 */

#ifndef MODULES_DEBUG_MISC_H
# define MODULES_DEBUG_MISC_H 1

/*
 * ---------- includes --------------------------------------------------------
 */

# include "kaneton.h"

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * misc.c
 */

t_uint8 hex_to_uint8(t_uint8* str);

t_uint8* uint8_to_str(t_uint8 n, t_uint8* to);

#endif /* !MODULES_DEBUG_MISC_H */
