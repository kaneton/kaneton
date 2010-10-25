/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/segment/multiple/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed oct 20 16:34:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_multiple_03(void)
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
      MULTIPLE_ALLOCATE(as, (i % 10) + 1, seg + i);

      for (j = 0; j < 64; j++)
	if (i != j && seg[i] == seg[j])
	  {
	    TEST_ERROR("some segments seem to overlap");
	    TEST_LEAVE();
	  }
    }

  MULTIPLE_ALLOCATE(as, 3, seg);
  MULTIPLE_ALLOCATE(as, 9, seg + 1);
  MULTIPLE_ALLOCATE(as, 1, seg + 2);
  MULTIPLE_ALLOCATE(as, 5, seg + 1);
  MULTIPLE_ALLOCATE(as, 7, seg);
  MULTIPLE_ALLOCATE(as, 4, seg + 3);

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
