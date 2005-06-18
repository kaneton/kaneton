/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strncpy.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:05:19 2005   mycure
 * last update   Sat Jun 18 15:53:05 2005   mycure
 */

#include <klibc/include/klibc.h>

char*			strncpy(char*		to,
				char*		from,
				size_t		n)
{
  u_int			i;

  for (i = 0; from[i] && i < n; i++)
    to[i] = from[i];

  if (i < n)
    to[i] = 0;

  return (to);
}
