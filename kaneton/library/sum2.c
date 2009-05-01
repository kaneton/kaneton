/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libdata/sum2.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [wed jul 26 19:18:50 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

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

  for (total += (u_int32_t)size, p = (u_char*)buf; size--; ++p)
    thecrc += *p;

  thecrc = (thecrc & 0xffff) + (thecrc >> 16);
  thecrc = (thecrc & 0xffff) + (thecrc >> 16);

  return (thecrc);
}
