/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/04/04.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [fri mar 17 15:37:09 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_04(void)
{
  t_tskid	task1;
  t_tskid	task2;
  t_asid	as;
  o_as*		o;

  TEST_ENTER;

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task1) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task2) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task1, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(as_give(as, task2) == ERROR_NONE, "error giving as\n");

  MY_ASSERT(as_get(as, &o) == ERROR_NONE,
	    "unable to get as\n");

  MY_ASSERT(o->tskid == task2, "Bad tskid field\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task1) == ERROR_NONE,
	    "failed to release task\n");

  MY_ASSERT(task_release(task2) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}