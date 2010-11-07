/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/test/tests/kaneton/as/give/give.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun nov  7 15:44:02 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "give.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_as_give(void)
{
  i_task		task1;
  i_task		task2;
  i_as			as;
  o_as*			o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task1) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task2) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task1, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (as_give(task2, as) != ERROR_NONE)
    TEST_ERROR("[as_give] error\n");

  if (as_get(as, &o) != ERROR_NONE)
    TEST_ERROR("[as_get] error\n");

  if (o->tskid != task2)
    TEST_ERROR("the address space's task identifier is invalid\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task1) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  if (task_release(task2) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
