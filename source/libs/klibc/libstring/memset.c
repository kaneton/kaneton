/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * memset.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:52:20 2005   mycure
 * last update   Sun Jun 19 22:26:58 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

void*			memset(void*				s,
			       int				c,
			       size_t				n)
{
  char*			d = s;
  u_int			i;

  for (i = 0; i < n; i++)
    d[i] = c;

  return (d);
}
