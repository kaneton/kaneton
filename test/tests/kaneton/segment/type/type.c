/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...E/test/tests/kaneton/segment/type/type.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 19:53:36 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "type.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_type(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  o_segment*		o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      2 * PAGESZ,
		      PERM_READ | PERM_EXEC,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_perms(seg, PERM_READ | PERM_WRITE) != ERROR_NONE)
    TEST_ERROR("[segment_perms] error\n");

  if (segment_type(seg, SEGMENT_TYPE_CATCH) != ERROR_NONE)
    TEST_ERROR("[segment_type] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o->perms != (PERM_READ | PERM_WRITE))
    TEST_ERROR("invalid segment's permissions\n");

  if (o->type != SEGMENT_TYPE_CATCH)
    TEST_ERROR("invalid segment's type\n");

  if (segment_perms(seg, ~(PERM_READ | PERM_WRITE | PERM_EXEC)) == ERROR_NONE)
    TEST_ERROR("[segment_perms] error: setting invalid permissions\n");

  if (segment_type(seg, (1 << 4)) == ERROR_NONE)
    TEST_ERROR("[segment_type] error: setting invalid type\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] errors\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
