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
 * updated       julien quintard   [sun jan 30 21:16:44 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "split.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

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

  if (segment_reserve(_kernel.as,
		      5 * ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
		     seg,
		     ___kaneton$pagesz,
		     REGION_OPTION_NONE,
		     0,
		     4 * ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_split(_kernel.as, reg, 3 * ___kaneton$pagesz, &left, &right) != STATUS_OK)
    TEST_ERROR("[region_split] error");

  if (region_get(_kernel.as, left, &o) != STATUS_OK)
    TEST_ERROR("[region_get] error: unable to retrieve the split "
	       "region's left part");

  if (o->id != left)
    TEST_ERROR("invalid region's identifier after split");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after split");

  if (o->offset != ___kaneton$pagesz)
    TEST_ERROR("invalid region's offset after split");

  if (o->size != 3 * ___kaneton$pagesz)
    TEST_ERROR("invalid region's size after split");

  if (region_get(_kernel.as, right, &o) != STATUS_OK)
    TEST_ERROR("[region_get] error: unable to retrieve the split "
	       "region's right part");

  if (o->id != right)
    TEST_ERROR("invalid region's identifier after split");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after split");

  if (o->offset != 4 * ___kaneton$pagesz)
    TEST_ERROR("invalid region's offset after split");

  if (o->size != ___kaneton$pagesz)
    TEST_ERROR("invalid region's size after split");

  if (region_coalesce(_kernel.as, left, right, &reg) != STATUS_OK)
    TEST_ERROR("[region_coalesce] error");

  if (region_get(_kernel.as, reg, &o) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier after coalescingt");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after coalescing");

  if (o->offset != ___kaneton$pagesz)
    TEST_ERROR("invalid region's offset after coalescing");

  if (o->size != 4 * ___kaneton$pagesz)
    TEST_ERROR("invalid region's size after coalescing");

  TEST_SIGNATURE(fr309iearbg9hh34h);

  TEST_LEAVE();
}
