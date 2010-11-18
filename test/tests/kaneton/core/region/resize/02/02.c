/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/region/resize/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [wed nov 17 13:29:10 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_region_resize_02(void)
{
  i_segment		seg;
  i_region		reg;
  o_region*		o;
  t_uint8*		p;

  TEST_ENTER();

  if (segment_reserve(kasid,
		      10 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(kasid,
		     seg,
		     PAGESZ,
		     REGION_OPT_NONE,
		     0,
		     2 * PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_resize(kasid, reg, 20 * PAGESZ, &reg) == ERROR_NONE)
    TEST_ERROR("[region_resize] error: out of bound\n");

  if (region_release(kasid, reg) != ERROR_NONE)
    TEST_ERROR("[region_release] error\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
