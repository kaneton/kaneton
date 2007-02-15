/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/01/01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sat oct 14 17:04:18 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_01(void)
{
  i_task	task;
  t_asid	as;
  o_as*		o;

  TEST_ENTER();

  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  ASSERT(as_get(as, &o) == ERROR_NONE,
	    "unable to get as\n");

  ASSERT(o->asid == as, "Bad asid field\n");
  ASSERT(o->tskid == task, "Bad tskid field\n");

  ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
