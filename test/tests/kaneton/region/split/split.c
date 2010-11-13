/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../test/tests/kaneton/region/split/split.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri nov 12 21:50:11 2010]
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

void			test_region_split(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_region		reg;
  o_region*		o;
  i_region		left;
  i_region		right;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      5 * PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(as,
		     seg,
		     PAGESZ,
		     REGION_OPT_NONE,
		     0,
		     4 * PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_split(as, reg, 3 * PAGESZ, &left, &right) != ERROR_NONE)
    TEST_ERROR("[region_split] error\n");

  if (region_get(as, left, &o) != ERROR_NONE)
    TEST_ERROR("[region_get] error: unable to retrieve the split region's left part\n");

  if (o->regid != left)
    TEST_ERROR("invalid region's identifier after split\n");

  if (o->segid != seg)
    TEST_ERROR("invalid region's segment identifier after split\n");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after split\n");

  if (o->size != 3 * PAGESZ)
    TEST_ERROR("invalid region's size after split\n");

  if (region_get(as, right, &o) != ERROR_NONE)
    TEST_ERROR("[region_get] error: unable to retrieve the split region's right part\n");

  if (o->regid != right)
    TEST_ERROR("invalid region's identifier after split\n");

  if (o->segid != seg)
    TEST_ERROR("invalid region's segment identifier after split\n");

  if (o->offset != 4 * PAGESZ)
    TEST_ERROR("invalid region's offset after split\n");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid region's size after split\n");

  if (region_coalesce(as, left, right, &reg) != ERROR_NONE)
    TEST_ERROR("[region_coalesce] error\n");

  if (region_get(as, reg, &o) != ERROR_NONE)
    TEST_ERROR("[region_get] error\n");

  if (o->regid != reg)
    TEST_ERROR("invalid region's identifier after coalescingt\n");

  if (o->segid != seg)
    TEST_ERROR("invalid region's segment identifier after coalescing\n");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after coalescing\n");

  if (o->size != 4 * PAGESZ)
    TEST_ERROR("invalid region's size after coalescing\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
