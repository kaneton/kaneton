/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libstring/bcmp.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:56 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

int			bcmp(const void*			b1,
			     const void*			b2,
			     size_t				length)
{
  const char*		p1 = b1;
  const char*		p2 = b2;

  if (length == 0)
    return(0);

  do
    {
      if (*p1++ != *p2++)
	break;
    } while (--length);

  return (length);
}
