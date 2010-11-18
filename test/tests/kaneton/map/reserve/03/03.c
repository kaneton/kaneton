/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...E/test/tests/kaneton/map/reserve/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 09:12:42 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_map_reserve_03(void)
{
  t_vaddr		addr;
  t_uint32		i;
  t_uint32		j;
  t_uint8*		p;

  TEST_ENTER();

  for (i = 0; i < 200; i++)
    {
      if (map_reserve(kasid,
		      MAP_OPT_NONE,
		      ((i % 100) + 1) * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &addr) != ERROR_NONE)
	TEST_ERROR("[map_reserve] error\n");

      for (j = 0, p = (t_uint8*)addr;
           j < ((i % 100) + 1) * PAGESZ;
           p++, j++)
        {
          *p = 0x0d;

          if (*p != 0x0d)
	    TEST_ERROR("the data read is different from the one written\n");
        }

      if (map_release(kasid, addr) != ERROR_NONE)
	TEST_ERROR("[map_release] error\n");
    }

  TEST_LEAVE();
}
