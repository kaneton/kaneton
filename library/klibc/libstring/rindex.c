/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libstring/rindex.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:09 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			rindex(const char*			p,
			       int				ch)
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
