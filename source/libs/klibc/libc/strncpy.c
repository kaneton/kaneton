/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strncpy.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:05:19 2005   mycure
 * last update   Sat Apr 23 17:00:04 2005   mycure
 */

#include <klibc/include/klibc.h>

char			*strncpy(char		*to,
				 char		*from,
				 size_t		n)
{
  u_int			i;

  for (i = 0; from[i] && i < n; i++)
    to[i] = from[i];

  return (to);
}
