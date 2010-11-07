/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/segment/permissions/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 14:47:01 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_permissions_04(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  t_uint32		i = 0x41424344;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as, PAGESZ, PERM_READ, &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_write(seg, 0, &i, sizeof(t_uint32)) == ERROR_NONE)
    TEST_ERROR("[segment_write] error: allowed writing to a read "
	       "only segment\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
