/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/segment/multiple/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 15:42:15 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_multiple_01(void)
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

  MULTIPLE_ALLOCATE(as, 2, seg);
  MULTIPLE_ALLOCATE(as, 8, seg + 1);
  MULTIPLE_ALLOCATE(as, 2, seg + 2);
  MULTIPLE_ALLOCATE(as, 5, seg + 1);
  MULTIPLE_ALLOCATE(as, 8, seg);
  MULTIPLE_ALLOCATE(as, 4, seg + 3);

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
