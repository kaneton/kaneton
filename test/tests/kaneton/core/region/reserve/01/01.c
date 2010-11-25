/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/core/region/reserve/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 24 09:36:00 2010]
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
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(_kernel->as,
		     seg,
		     0,
		     REGION_OPTION_NONE,
		     0,
		     2 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error\n");

  if (region_get(_kernel->as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error\n");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier\n");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier\n");

  if (o->offset != 0)
    TEST_ERROR("invalid region's offset\n");

  if (o->size != 2 * PAGESZ)
    TEST_ERROR("invalid region's size\n");

  TEST_LEAVE();
}
