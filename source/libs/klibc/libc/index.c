/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * index.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:51:11 2005   mycure
 * last update   Sat Apr 23 16:57:44 2005   mycure
 */

#include <klibc/include/klibc.h>

char			*index(const char	*p,
			       int		ch)
{
  u_int			i;

  for (i = 0; p[i]; i++)
    if (p[i] == ch)
      return ((char *) p + i);

  return (NULL);
}
