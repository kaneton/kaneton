/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strcmp.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:53:44 2005   mycure
 * last update   Sun Jun 19 22:27:46 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

int			strcmp(const char*			s1,
			       const char*			s2)
{
  u_int			i;
  u_int			j;

  for (i = 0, j = 0; s1[i] && s2[j]; i++, j++)
    if (s1[i] != s2[j])
      return (s1[i] - s2[j]);

  if (s1[i] != s2[j])
    return (s1[i] - s2[j]);

  return (0);
}