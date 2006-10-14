/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/02/02.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [tue apr 11 17:27:13 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_02(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_regid	reg;
  o_segment*	oseg;
  o_region*	oreg;

  TEST_ENTER();

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    1200 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(region_reserve(as,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   1200 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  MY_ASSERT(segment_get(seg, &oseg) == ERROR_NONE,
	    "error getting segment\n");

  MY_ASSERT(oseg->segid == seg, "Bad segid field in segment\n");
  MY_ASSERT(oseg->asid == as, "Bad asid field in segment\n");

  MY_ASSERT(region_get(as, reg, &oreg) == ERROR_NONE,
	    "error getting region\n");

  MY_ASSERT(oreg->regid == reg, "Bad regid field in region\n");
  MY_ASSERT(oreg->segid == seg, "Bad segid field in region\n");

  MY_ASSERT(region_release(as, reg) == ERROR_NONE,
	    "failed to release region\n");

  MY_ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release segment\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
