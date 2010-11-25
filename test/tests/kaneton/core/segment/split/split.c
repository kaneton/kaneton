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
 * updated       julien quintard   [wed nov 24 09:44:46 2010]
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

void			test_core_segment_split(void)
{
  i_segment		seg;
  i_segment		seg2;
  o_segment*		o1;
  o_segment*		o2;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      3 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_split(seg, 2 * PAGESZ, &seg, &seg2) != ERROR_OK)
    TEST_ERROR("[segment_split] error\n");

  if (segment_get(seg, &o1) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (segment_get(seg2, &o2) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (o2->address != (o1->address + 2 * PAGESZ))
    TEST_ERROR("invalid segments' address\n");

  if (o1->id != seg)
    TEST_ERROR("invalid segment's identifier after split\n");

  if (o1->as != _kernel->as)
    TEST_ERROR("invalid segment's address space identifier after split\n");

  if (o1->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after split\n");

  if (o1->size != 2 * PAGESZ)
    TEST_ERROR("invalid segment's size after split\n");

  if (o1->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after split\n");

  if (o2->id != seg2)
    TEST_ERROR("invalid segment's identifier after split\n");

  if (o2->as != _kernel->as)
    TEST_ERROR("invalid segment's address space identifier after split\n");

  if (o2->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after split\n");

  if (o2->size != PAGESZ)
    TEST_ERROR("invalid segment's size after split\n");

  if (o2->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after split\n");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg2) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
