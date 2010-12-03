/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...test/tests/kaneton/core/region/get/get.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:48:09 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "get.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_get(void)
{
  i_segment		seg;
  i_region		reg;
  o_region*		o;

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

  if (region_get(_kernel->as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset");

  if (o->size != 2 * PAGESZ)
    TEST_ERROR("invalid region's size");

  TEST_SIGNATURE(8t3uiegisijf2w93w);

  TEST_LEAVE();
}
