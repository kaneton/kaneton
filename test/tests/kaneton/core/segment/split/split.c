/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/core/segment/split/split.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:09:12 2011]
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

void			test_core_segment_split(void)
{
  i_segment		seg;
  i_segment		seg2;
  o_segment*		o1;
  o_segment*		o2;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      3 * ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_split(seg, 2 * ___kaneton$pagesz, &seg, &seg2) != STATUS_OK)
    TEST_ERROR("[segment_split] error");

  if (segment_get(seg, &o1) != STATUS_OK)
    TEST_ERROR("[segment_get] error");

  if (segment_get(seg2, &o2) != STATUS_OK)
    TEST_ERROR("[segment_get] error");

  if (o2->address != (o1->address + 2 * ___kaneton$pagesz))
    TEST_ERROR("invalid segments' address");

  if (o1->id != seg)
    TEST_ERROR("invalid segment's identifier after split");

  if (o1->as != _kernel.as)
    TEST_ERROR("invalid segment's address space identifier after split");

  if (o1->options != SEGMENT_OPTION_NONE)
    TEST_ERROR("invalid segment's options after split");

  if (o1->size != 2 * ___kaneton$pagesz)
    TEST_ERROR("invalid segment's size after split");

  if (o1->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after split");

  if (o2->id != seg2)
    TEST_ERROR("invalid segment's identifier after split");

  if (o2->as != _kernel.as)
    TEST_ERROR("invalid segment's address space identifier after split");

  if (o2->options != SEGMENT_OPTION_NONE)
    TEST_ERROR("invalid segment's options after split");

  if (o2->size != ___kaneton$pagesz)
    TEST_ERROR("invalid segment's size after split");

  if (o2->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after split");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg2) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(vsdlawogjjgeoihj4e3h);

  TEST_LEAVE();
}
