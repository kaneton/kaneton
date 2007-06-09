/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libstring/strcat.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:05 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			strcat(char*				s,
			       const char*			append)
{
  u_int			i;

  for (i = 0; s[i]; i++)
    ;

  for (; s[i] && *append; i++, append++)
    s[i] = *append;
  s[i] = 0;

  return (s);
}
