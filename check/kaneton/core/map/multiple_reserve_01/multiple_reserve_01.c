/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/2008-K1/kaneton/check/kaneton/core/map/multiple_reserve_01/multiple_reserve_01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun mar  4 17:47:36 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_map_multiple_reserve_01(void)
{
  t_vaddr	addr;
  t_uint32	i;
  t_uint32	j;
  t_uint8*	p;

  TEST_ENTER();

  for (i = 0; i < 200; i++)
    {
      assert(map_reserve(kasid,
			    MAP_OPT_NONE,
			    ((i % 100) + 1) * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &addr) == ERROR_NONE,
		"error reserving a map\n");

      for (j = 0, p = (t_uint8*)addr;
	   j < ((i % 100) + 1) * PAGESZ;
	   p++, j++)
	{
	  *p = 0x0d;
	  assert(*p == 0x0d, "integrity check failed\n");
	}
    }

  TEST_LEAVE();
}
