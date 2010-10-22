/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/segment/permissions/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed oct 20 17:07:22 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_permissions_01(void)
{
  i_task		task;
  o_segment*		o;
  i_as			as;
  i_segment		seg;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    printf("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    printf("[as_reserve] error\n");

  if (segment_reserve(as, PAGESZ, PERM_READ | PERM_EXEC, &seg) != ERROR_NONE)
    printf("[segment_reserve] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    printf("[segment_get] error\n");

  if (o->perms != (PERM_READ | PERM_EXEC))
    printf("invalid segment's permissions\n");

  if (segment_perms(seg, PERM_READ | PERM_WRITE) != ERROR_NONE)
    printf("[segment_perms] error\n");

  if (o->perms != (PERM_READ | PERM_WRITE))
    printf("invalid segment's permissions\n");

  if (as_release(as) != ERROR_NONE)
    printf("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    printf("[task_release] error\n");

  TEST_LEAVE();
}
