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
 * updated       julien quintard   [thu nov 18 16:33:04 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "split.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

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

  if (segment_reserve(kasid,
		      3 * PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_split(seg, 2 * PAGESZ, &seg, &seg2) != ERROR_NONE)
    TEST_ERROR("[segment_split] error\n");

  if (segment_get(seg, &o1) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (segment_get(seg2, &o2) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o2->address != (o1->address + 2 * PAGESZ))
    TEST_ERROR("invalid segments' address\n");

  if (o1->segid != seg)
    TEST_ERROR("invalid segment's identifier after split\n");

  if (o1->asid != kasid)
    TEST_ERROR("invalid segment's address space identifier after split\n");

  if (o1->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after split\n");

  if (o1->size != 2 * PAGESZ)
    TEST_ERROR("invalid segment's size after split\n");

  if (o1->perms != PERM_READ)
    TEST_ERROR("invalid segment's permissions after split\n");

  if (o2->segid != seg2)
    TEST_ERROR("invalid segment's identifier after split\n");

  if (o2->asid != kasid)
    TEST_ERROR("invalid segment's address space identifier after split\n");

  if (o2->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after split\n");

  if (o2->size != PAGESZ)
    TEST_ERROR("invalid segment's size after split\n");

  if (o2->perms != PERM_READ)
    TEST_ERROR("invalid segment's permissions after split\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg2) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
