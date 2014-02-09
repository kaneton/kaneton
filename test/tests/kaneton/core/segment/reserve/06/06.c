/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/segment/reserve/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun dec  5 14:00:30 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_reserve_06(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg[64];
  t_uint32		i;
  t_uint32		j;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  for (i = 0; i < 64; i++)
    {
      TEST_ALLOCATE(as, (i % 10) + 1, seg + i);

      for (j = 0; j < 64; j++)
	if (i != j && seg[i] == seg[j])
	  TEST_ERROR("some segments seem to overlap");
    }

  TEST_ALLOCATE(as, 3, seg);
  TEST_ALLOCATE(as, 9, seg + 1);
  TEST_ALLOCATE(as, 1, seg + 2);
  TEST_ALLOCATE(as, 5, seg + 1);
  TEST_ALLOCATE(as, 7, seg);
  TEST_ALLOCATE(as, 4, seg + 3);

  if (as_release(as) != STATUS_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != STATUS_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(230i0f9i32qgi3g34);

  TEST_LEAVE();
}
