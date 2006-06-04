/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libdata/malloc/02/02.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [sun jun  4 19:54:49 2006]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * many calls to malloc with different size.
 */

void		check_malloc_02(void)
{
  void		*p[100];
  int		i;
  size_t	sz;

  TEST_ENTER(1);

  for (i = 0; i < 100; i++)
    {
      sz = (i + 1) % 42;
      p[i] = check_init_memory(malloc(sz), sz);
    }
  for (i = 0; i < 100; i++)
    {
      sz = (i + 1) % 42;
      check_memory(p[i], sz);
      if (p[i])
	free(p[i]);
    }

  TEST_LEAVE;
}
