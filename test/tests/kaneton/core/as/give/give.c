/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...E/test/tests/kaneton/core/as/give/give.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:41:29 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "give.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_give(void)
{
  i_task		task1;
  i_task		task2;
  i_as			as;
  o_as*			o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task1) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task2) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task1, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (as_give(as, task2) != ERROR_OK)
    TEST_ERROR("[as_give] error");

  if (as_get(as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error");

  if (o->task != task2)
    TEST_ERROR("the address space's task identifier is invalid");

  TEST_SIGNATURE(9ri2309skdporgke9);

  TEST_LEAVE();
}
