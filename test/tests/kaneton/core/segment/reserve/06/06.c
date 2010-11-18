/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/segment/reserve/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov  8 09:47:03 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_reserve_06(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg[64];
  t_uint32		i;
  t_uint32		j;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

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

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
