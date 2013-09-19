/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/misc.c
 *
 * created       julio guerra   [mon sept  15 13:45:13 2013]
 * updated       julio guerra   [web sept  18 22:50:33 2013]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements misc functions used by the gdb server.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "misc.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * Conversion to an unsigned byte from a lower case hexadecimal string
 */
t_uint8 hex_to_uint8(t_uint8* str)
{
  t_uint8 r = 0;
  t_uint8 c;

  while (*str)
  {
    c = (*str > '9') ? *str - 'a' + 10 : *str - '0';
    r = 16 * r + c;
    ++str;
  }

  return (r);
}

/*
 * Conversion to a lower case hexadecimal string of an unsigned byte
 */
t_uint8* uint8_to_str(t_uint8 n, t_uint8* to)
{
  to[0] = n / 16;
  to[1] = n % 16;

  to[0] += (to[0] > 9) ? 'a' - 10 : '0';
  to[1] += (to[1] > 9) ? 'a' - 10 : '0';

  return (to);
}
