/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/segment/reserve/07/07.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun dec  5 14:00:39 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "07.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_reserve_07(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg[4];

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  TEST_ALLOCATE(as, 2, seg);
  TEST_ALLOCATE(as, 8, seg + 1);
  TEST_ALLOCATE(as, 2, seg + 2);

  if (segment_release(seg[1]) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_ALLOCATE(as, 5, seg + 1);

  if (segment_release(seg[0]) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_ALLOCATE(as, 8, seg);
  TEST_ALLOCATE(as, 4, seg + 3);

  if (segment_release(seg[0]) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg[1]) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg[2]) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg[3]) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (as_release(as) != STATUS_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != STATUS_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(r3982804tg3g43);

  TEST_LEAVE();
}
