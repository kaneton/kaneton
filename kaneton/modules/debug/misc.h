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

t_uint8  hchar_to_uint8(t_uint8 c);

t_uint8  hstr_to_uint8(t_uint8* str);

t_uint8* uint8_to_hstr(t_uint8 n, t_uint8* to);

t_uint8* uint32_to_hstr(t_uint32 x, t_uint8* to);

t_uint32 hstr_to_uint32(t_uint8* str);

t_uint32 str_to_uint32(t_uint8* str);

t_boolean is_hchar(t_uint8 c);

#endif /* !MODULES_DEBUG_MISC_H */
