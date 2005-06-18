/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * rindex.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:53:05 2005   mycure
 * last update   Sat Jun 18 15:50:18 2005   mycure
 */

#include <klibc/include/klibc.h>

char*			rindex(const char*	p,
			       int		ch)
{
  int			loccur;
  u_int			i;

  for (loccur = -1, i = 0; p[i]; i++)
    if (p[i] == ch)
      loccur = i;

  if (loccur != -1)
    return ((char *) p + loccur);

  return (NULL);
}
