/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * sum2.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:56:44 2005   mycure
 * last update   Sun Jun 19 22:29:45 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

u_int32_t		sum2(char				*buf,
			     int				size)
{
  register u_int32_t	thecrc;
  register u_int32_t	total;
  register u_char	*p;

  /*
   * Draft 8 POSIX 1003.2:
   *
   *      s = sum of all bytes
   *      r = s % 2^16 + (s % 2^32) / 2^16
   * thecrc = (r % 2^16) + r / 2^16
   */

  thecrc = total = 0;

  for (total += size, p = buf; size--; ++p)
    thecrc += *p;

  thecrc = (thecrc & 0xffff) + (thecrc >> 16);
  thecrc = (thecrc & 0xffff) + (thecrc >> 16);

  return (thecrc);
}
