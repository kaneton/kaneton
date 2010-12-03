/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../tests/kaneton/core/region/split/split.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:51:11 2010]
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
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel->as,
		     seg,
		     PAGESZ,
		     REGION_OPTION_NONE,
		     0,
		     4 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_split(_kernel->as, reg, 3 * PAGESZ, &left, &right) != ERROR_OK)
    TEST_ERROR("[region_split] error");

  if (region_get(_kernel->as, left, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error: unable to retrieve the split "
	       "region's left part");

  if (o->id != left)
    TEST_ERROR("invalid region's identifier after split");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after split");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after split");

  if (o->size != 3 * PAGESZ)
    TEST_ERROR("invalid region's size after split");

  if (region_get(_kernel->as, right, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error: unable to retrieve the split "
	       "region's right part");

  if (o->id != right)
    TEST_ERROR("invalid region's identifier after split");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after split");

  if (o->offset != 4 * PAGESZ)
    TEST_ERROR("invalid region's offset after split");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid region's size after split");

  if (region_coalesce(_kernel->as, left, right, &reg) != ERROR_OK)
    TEST_ERROR("[region_coalesce] error");

  if (region_get(_kernel->as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier after coalescingt");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after coalescing");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after coalescing");

  if (o->size != 4 * PAGESZ)
    TEST_ERROR("invalid region's size after coalescing");

  TEST_SIGNATURE(fr309iearbg9hh34h);

  TEST_LEAVE();
}
