/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strcpy.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:53:59 2005   mycure
 * last update   Sat Apr 23 16:59:22 2005   mycure
 */

#include <klibc/include/klibc.h>

char			*strcpy(char		*to,
				char		*from)
{
  u_int			i;

  for (i = 0; from[i]; i++)
    to[i] = from[i];
  to[i] = 0;

  return (to);
}
