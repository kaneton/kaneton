/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/core/map/reserve/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sun jan 30 21:10:58 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_map_reserve_02(void)
{
  t_vaddr		addr;
  t_uint32		i;
  t_uint32		j;
  t_uint8*		p;

  TEST_ENTER();

  for (i = 0; i < 10; i++)
    {
      if (map_reserve(_kernel.as,
		      MAP_OPTION_NONE,
		      ((i % 100) + 1) * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &addr) != STATUS_OK)
	TEST_ERROR("[map_reserve] error");

      for (j = 0, p = (t_uint8*)addr;
           j < ((i % 100) + 1) * ___kaneton$pagesz;
           p++, j++)
        {
          *p = 0x0d;

          if (*p != 0x0d)
	    TEST_ERROR("the data read is different from the one written");
        }

      if (map_release(_kernel.as, addr) != STATUS_OK)
	TEST_ERROR("[map_release] error");
    }

  TEST_SIGNATURE(i9t04okafk3t4hg3);

  TEST_LEAVE();
}
