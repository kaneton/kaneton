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
 * updated       julien quintard   [tue apr 12 20:49:02 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_reserve_01(void)
{
  i_segment		seg;
  i_region		reg;
  o_region*		o;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
		     seg,
		     0,
		     REGION_OPTION_NONE,
		     0,
		     2 * ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as, reg, &o) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier");

  if (o->offset != 0)
    TEST_ERROR("invalid region's offset");

  if (o->size != 2 * ___kaneton$pagesz)
    TEST_ERROR("invalid region's size");

  TEST_SIGNATURE(0f9iwoiwfsmfwiojrw2);

  TEST_LEAVE();
}
