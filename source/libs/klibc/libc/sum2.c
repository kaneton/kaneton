/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * sum2.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:56:44 2005   mycure
 * last update   Sat Apr 23 17:02:05 2005   mycure
 */

#include <klibc/include/klibc.h>

u_int32_t		sum2(char		*buf,
			     int		size)
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
