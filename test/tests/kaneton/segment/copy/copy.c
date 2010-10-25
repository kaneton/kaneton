/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...E/test/tests/kaneton/segment/copy/copy.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed oct 20 15:07:42 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "copy.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_copy(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_segment		seg_ref;
  t_uint32		i;
  static t_uint8	buff[PAGESZ];

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg_ref) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  if (segment_write(seg_ref, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_write] error\n");

  if (segment_copy(seg, 0, seg_ref, 0,
		   PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_copy] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  if (segment_read(seg, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_read] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      if (buff[i] != (i * 4 - 1) % 256)
	TEST_ERROR("invalid data in the segment\n");
    }

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
