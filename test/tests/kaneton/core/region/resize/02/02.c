/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/core/region/resize/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sun jan 30 21:13:22 2011]
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

void			test_core_region_resize_02(void)
{
  i_segment		seg;
  i_region		reg;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      10 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
		     seg,
		     ___kaneton$pagesz,
		     REGION_OPTION_NONE,
		     0,
		     2 * ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_resize(_kernel.as, reg, 20 * ___kaneton$pagesz, &reg) == STATUS_OK)
    TEST_ERROR("[region_resize] error: out of bound");

  if (region_release(_kernel.as, reg) != STATUS_OK)
    TEST_ERROR("[region_release] error");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(3w2f0w9agi09h09hiw3);

  TEST_LEAVE();
}
