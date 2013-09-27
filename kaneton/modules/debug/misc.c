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
 * Conversion to an unsigned byte from a lower case hexadecimal string (XX)
 */
t_uint8 hstr_to_uint8(t_uint8* str)
{
  t_uint8 r;

  r = hchar_to_uint8(str[0]) * 16;
  r += hchar_to_uint8(str[1]);

  return (r);
}

/*
 * Unsigned half byte (0..15) to hexadecimal string character (0..f)
 */
static t_uint8 uint4_to_hchar(t_uint8 x)
{
  assert(x < 16);
  return (x + (x < 10 ? '0' : 'a' - 10));
}

/*
 * Hexadecimal string character (0..F || 0..f) to unsigned byte
 */
t_uint8 hchar_to_uint8(t_uint8 c)
{
  t_uint8 base;

  if ('0' <= c && c <= '9')
    base = '0';
  else if ('a' <= c && c <= 'f')
    base = 'a' - 10;
  else
  {
    assert('A' <= c && c <= 'F');
    base = 'A' - 10;
  }

  return (c - base);
}

/*
 * Unsigned byte (0..255) to hexadecimal string character (00..ff)
 */
t_uint8* uint8_to_hstr(t_uint8 x, t_uint8* str)
{
  str[0] = uint4_to_hchar(x / 16);
  str[1] = uint4_to_hchar(x % 16);

  return (str);
}

/*
 * Unsigned word to hexadecimal string (XXXXXXXX)
 */
t_uint8* uint32_to_hstr(t_uint32 x, t_uint8* to)
{
  unsigned int n;

  for (n = 0; n < 8; ++n)
    to[n] = uint4_to_hchar((x >> (28 - n * 4)) & 0xf);

  return (to);
}


/*
 * Hexadecimal string (XXXXXXXX) to unsigned word
 */
t_uint32 hstr_to_uint32(t_uint8* str)
{
  t_uint32 x = 0;

  assert(str);

  while (*str)
  {
    x = 16 * x + hchar_to_uint8(*str);
    ++str;
  }

  return (x);
}

/*
 * String character (0..9) to unsigned byte
 */
static t_uint8 char_to_uint8(t_uint8 c)
{
  t_uint8 base;

  assert('0' <= c && c <= '9');

  return (c - '0');
}

/*
 * String to unsigned word
 */
t_uint32 str_to_uint32(t_uint8* str)
{
  t_uint32 x = 0;

  assert(str);

  while (*str)
  {
    x = 10 * x + char_to_uint8(*str);
    ++str;
  }

  return (x);
}

/*
 * Return true if argument c is a valid hexadecimal character
 */
t_boolean is_hchar(t_uint8 c)
{
  return (('0' <= c && c <= '9')
          || ('a' <= c && c <= 'f')
          || ('A' <= c && c <= 'F'));
}
