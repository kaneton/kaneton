/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/06/06.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [fri mar 24 16:22:17 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_06(void)
{
  t_tskid	task;
  t_tskid	forked;
  t_asid	as;
  t_asid	cloned;
  t_segid	seg;
  t_regid	reg;

  TEST_ENTER;

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &forked) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    1024 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(region_reserve(as,
			   seg,
			   0,
			   REGION_OPT_FORCE,
			   1024 * PAGESZ,
			   1024 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  MY_ASSERT(as_clone(forked, as, &cloned) == ERROR_NONE, "as_clone failed\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
