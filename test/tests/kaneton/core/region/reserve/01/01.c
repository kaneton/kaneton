/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...ests/kaneton/core/region/reserve/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:52:28 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_reserve_01(void)
{
  i_segment		seg;
  i_region		reg;
  o_region*		o;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      2 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel->as,
		     seg,
		     0,
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

  if (o->offset != 0)
    TEST_ERROR("invalid region's offset");

  if (o->size != 2 * PAGESZ)
    TEST_ERROR("invalid region's size");

  TEST_SIGNATURE(0f9iwoiwfsmfwiojrw2);

  TEST_LEAVE();
}
