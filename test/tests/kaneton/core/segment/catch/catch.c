/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...tests/kaneton/core/segment/catch/catch.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:52:49 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "catch.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_catch(void)
{
  i_task		task1;
  i_task		task2;
  i_as			as1;
  i_as			as2;
  i_segment		seg;
  o_segment*		o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task1) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task2) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task1, &as1) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (as_reserve(task2, &as2) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (segment_reserve(as1,
		      2 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_type(seg, SEGMENT_TYPE_CATCH) != ERROR_OK)
    TEST_ERROR("[segment_type] unable to set the segment as catchable");

  if (segment_catch(as2, seg) != ERROR_OK)
    TEST_ERROR("[segment_catch] error");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error");

  if (o->as != as2)
    TEST_ERROR("invalid address space id after catch");

  if (o->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid type field after catch");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  if (as_release(as1) != ERROR_OK)
    TEST_ERROR("[as_release] error");

  if (as_release(as2) != ERROR_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task1) != ERROR_OK)
    TEST_ERROR("[task_release] error");

  if (task_release(task2) != ERROR_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(r93i29egih34i3);

  TEST_LEAVE();
}
