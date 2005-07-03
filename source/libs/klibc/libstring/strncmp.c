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
 * last update   Sun Jun 19 22:28:37 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

int			strncmp(const char*			s1,
				const char*			s2,
				size_t				n)
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
