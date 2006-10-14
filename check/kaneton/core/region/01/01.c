/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_01(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_regid	reg;
  o_region*	o;

  TEST_ENTER();

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    2 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(region_reserve(as,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  MY_ASSERT(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  MY_ASSERT(o->regid == reg, "Bad regid field\n");
  MY_ASSERT(o->segid == seg, "Bad segid field\n");
  MY_ASSERT(o->address == (t_vaddr)reg, "Bad address field\n");
  MY_ASSERT(o->offset == 0, "Bad offset field\n");
  MY_ASSERT(o->size == 2 * PAGESZ, "Bad size field\n");

  MY_ASSERT(region_release(as, reg) == ERROR_NONE, "error releasing region\n");

  MY_ASSERT(region_reserve(as,
			   seg,
			   PAGESZ,
			   REGION_OPT_FORCE,
			   0x40000000,
			   PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  MY_ASSERT(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  MY_ASSERT(o->regid == reg, "Bad regid field\n");
  MY_ASSERT(o->segid == seg, "Bad segid field\n");
  MY_ASSERT(o->address == (t_vaddr)reg, "Bad address field\n");
  MY_ASSERT(o->offset == PAGESZ, "Bad offset field\n");
  MY_ASSERT(o->size == PAGESZ, "Bad size field\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
