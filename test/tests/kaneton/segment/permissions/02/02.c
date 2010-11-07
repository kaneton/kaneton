/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/segment/permissions/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 14:45:20 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_permissions_02(void)
{
  i_task		task;
  o_segment*		o;
  i_as			as;
  i_segment		seg;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as, PAGESZ, PERM_READ | PERM_WRITE, &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o->perms != (PERM_READ | PERM_WRITE))
    TEST_ERROR("invalid segment's permissions\n");

  if (segment_perms(seg, PERM_READ) != ERROR_NONE)
    TEST_ERROR("[segment_perms] error\n");

  if (segment_perms(seg, PERM_READ | PERM_EXEC) != ERROR_NONE)
    TEST_ERROR("[segment_perms] error\n");

  if (o->perms != (PERM_READ | PERM_EXEC))
    TEST_ERROR("invalid segment's permissions\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
