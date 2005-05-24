/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * memcpy.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:51:54 2005   mycure
 * last update   Sat Apr 23 16:58:15 2005   mycure
 */

#include <klibc/include/klibc.h>

void			*memcpy(void		*dest,
				const void	*src,
				size_t		n)
{
  char			*d = dest;
  const char		*s = src;
  u_int			i;

  if ((n == 0) || (src == dest))
    return (NULL);

  /*
   * XXX bad performances
   */

  for (i = 0; i < n; i++)
    d[i] = s[i];

  return (d);
}
