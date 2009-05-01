/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libstring/memcmp.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:23 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- functions -------------------------------------------------------
 */

int			memcmp(const void*			s1,
			       const void*			s2,
			       size_t				n)
{
  if (n != 0)
    {
      const u_char*	p1 = s1;
      const u_char*	p2 = s2;

      do
        {
          if (*p1++ != *p2++)
            return (*--p1 - *--p2);
	} while (--n != 0);
    }

  return (0);
}
