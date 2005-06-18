/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strncmp.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:55:34 2005   mycure
 * last update   Sat Jun 18 15:52:51 2005   mycure
 */

#include <klibc/include/klibc.h>

int			strncmp(const char*	s1,
				const char*	s2,
				size_t		n)
{
  u_int			i;
  u_int			j;

  for (i = 0, j = 0; s1[i] && s2[j] && i < n; i++, j++)
    if (s1[i] != s2[j])
      return (s1[i] - s2[j]);

  if (s1[i] != s2[j])
    return (s1[i] - s2[j]);

  return (0);
}
