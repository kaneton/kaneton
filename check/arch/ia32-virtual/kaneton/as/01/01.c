/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/01/01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [fri mar 17 15:25:23 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_01(void)
{
  t_tskid	task;
  t_asid	as;
  o_as*		o;

  TEST_ENTER;

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(as_get(as, &o) == ERROR_NONE,
	    "unable to get as\n");

  MY_ASSERT(o->tskid == task, "Bad tskid field\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
