/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...tests/kaneton/core/region/resize/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [mon nov 29 18:50:14 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_resize_02(void)
{
  i_segment		seg;
  i_region		reg;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      10 * PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel->as,
		     seg,
		     PAGESZ,
		     REGION_OPTION_NONE,
		     0,
		     2 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_resize(_kernel->as, reg, 20 * PAGESZ, &reg) == ERROR_OK)
    TEST_ERROR("[region_resize] error: out of bound");

  if (region_release(_kernel->as, reg) != ERROR_OK)
    TEST_ERROR("[region_release] error");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(3w2f0w9agi09h09hiw3);

  TEST_LEAVE();
}
