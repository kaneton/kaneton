/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/segment/reserve/07/07.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov  8 09:48:37 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "07.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_reserve_07(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg[4];

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  TEST_ALLOCATE(as, 2, seg);
  TEST_ALLOCATE(as, 8, seg + 1);
  TEST_ALLOCATE(as, 2, seg + 2);

  if (segment_release(seg[1]) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_ALLOCATE(as, 5, seg + 1);

  if (segment_release(seg[0]) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_ALLOCATE(as, 8, seg);
  TEST_ALLOCATE(as, 4, seg + 3);

  if (segment_release(seg[0]) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg[1]) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg[2]) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg[3]) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
