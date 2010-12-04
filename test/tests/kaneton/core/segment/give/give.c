/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/core/segment/give/give.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat dec  4 13:23:06 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "give.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_give(void)
{
  i_task		task1;
  i_task		task2;
  i_as			as1;
  i_as			as2;
  i_segment		seg;
  o_segment*		o;
  o_as*			oas;
  void*			chiche;

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

  if (segment_give(seg, as2) != ERROR_OK)
    TEST_ERROR("[segment_give] error");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error");

  if (o->as != as2)
    TEST_ERROR("invalid segment's address space after give");

  if (as_get(as1, &oas) != ERROR_OK)
    TEST_ERROR("[as_get] error");

  if (set_get(oas->segments, seg, &chiche) == ERROR_OK)
    TEST_ERROR("[set_get] error: the segment has not been removed from "
	       "the first address space during the give process\n");

  if (as_get(as2, &oas) != ERROR_OK)
    TEST_ERROR("[as_get] error");

  if (set_get(oas->segments, seg, &chiche) != ERROR_OK)
    TEST_ERROR("[set_get] error: the segment does not seem to have been added "
	       "to destination address space during the give process\n");

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

  TEST_SIGNATURE(34i90iaopwrkrekhgoph349);

  TEST_LEAVE();
}
