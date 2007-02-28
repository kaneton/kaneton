/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/map/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sat oct 14 17:54:00 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern i_as	kasid;

void		check_map_map_01(void)
{
  t_vaddr	addr;
  t_uint32	i;
  t_uint32	j;
  t_uint8*	p;

  TEST_ENTER();

  for (i = 0; i < 200; i++)
    {
      ASSERT(map_reserve(kasid,
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
	  ASSERT(*p == 0x0d, "integrity check failed\n");
	}

      ASSERT(map_release(kasid, addr) == ERROR_NONE,
		"error releasing a map\n");
    }

  TEST_LEAVE();
}
