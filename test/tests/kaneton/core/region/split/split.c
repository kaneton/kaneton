/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/region/split/split.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 24 09:37:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "split.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_split(void)
{
  i_segment		seg;
  i_region		reg;
  o_region*		o;
  i_region		left;
  i_region		right;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      5 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(_kernel->as,
		     seg,
		     PAGESZ,
		     REGION_OPTION_NONE,
		     0,
		     4 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error\n");

  if (region_split(_kernel->as, reg, 3 * PAGESZ, &left, &right) != ERROR_OK)
    TEST_ERROR("[region_split] error\n");

  if (region_get(_kernel->as, left, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error: unable to retrieve the split region's left part\n");

  if (o->id != left)
    TEST_ERROR("invalid region's identifier after split\n");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after split\n");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after split\n");

  if (o->size != 3 * PAGESZ)
    TEST_ERROR("invalid region's size after split\n");

  if (region_get(_kernel->as, right, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error: unable to retrieve the split region's right part\n");

  if (o->id != right)
    TEST_ERROR("invalid region's identifier after split\n");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after split\n");

  if (o->offset != 4 * PAGESZ)
    TEST_ERROR("invalid region's offset after split\n");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid region's size after split\n");

  if (region_coalesce(_kernel->as, left, right, &reg) != ERROR_OK)
    TEST_ERROR("[region_coalesce] error\n");

  if (region_get(_kernel->as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error\n");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier after coalescingt\n");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after coalescing\n");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after coalescing\n");

  if (o->size != 4 * PAGESZ)
    TEST_ERROR("invalid region's size after coalescing\n");

  TEST_LEAVE();
}
