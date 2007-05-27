/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/give_01/give_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:28:26 2007]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_give_01(void)
{
  i_task	task1;
  i_task	task2;
  t_asid	as;
  o_as*		o;

  TEST_ENTER();

  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task1) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task2) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(as_reserve(task1, &as) == ERROR_NONE, "error creating as\n");

  ASSERT(as_give(task2, as) == ERROR_NONE, "error giving as\n");

  ASSERT(as_get(as, &o) == ERROR_NONE,
	    "unable to get as\n");

  ASSERT(o->tskid == task2, "Bad tskid field after as_give\n");

  ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task1) == ERROR_NONE,
	    "failed to release task\n");

  ASSERT(task_release(task2) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
