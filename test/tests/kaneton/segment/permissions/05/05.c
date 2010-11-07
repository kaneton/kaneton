/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/segment/permissions/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 14:48:38 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_permissions_05(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_segment		seg_ref;
  t_uint32		i;
  char			buff[PAGESZ];

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
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  if (segment_write(seg_ref, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_write] error\n");

  if (segment_copy(seg, 0, seg_ref, 0, PAGESZ) == ERROR_NONE)
    TEST_ERROR("[segment_copy] error: allowed copy to a read only segment\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg_ref) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
