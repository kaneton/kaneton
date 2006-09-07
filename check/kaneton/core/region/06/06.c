/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/region/06/06.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [thu sep  7 23:32:32 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_06(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_regid	reg;
  o_region*	o;
  t_regid	left;
  t_regid	right;

  TEST_ENTER;

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    5 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(region_reserve(as,
			   seg,
			   PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   4 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  MY_ASSERT(region_split(as, reg, 3 * PAGESZ, &left, &right) == ERROR_NONE,
	    "error splitting region\n");

  MY_ASSERT(region_get(as, left, &o) == ERROR_NONE,
	    "error getting region after split\n");

  MY_ASSERT(o->regid == left, "Bad regid field after split\n");
  MY_ASSERT(o->segid == seg, "Bad segid field after split\n");
  MY_ASSERT(o->address == (t_vaddr)left, "Bad address field after split\n");
  MY_ASSERT(o->offset == PAGESZ, "Bad offset field after split\n");
  MY_ASSERT(o->size == 3 * PAGESZ, "Bad size field after split\n");

  MY_ASSERT(region_get(as, right, &o) == ERROR_NONE,
	    "error getting region after split\n");

  MY_ASSERT(o->regid == right, "Bad regid field after split\n");
  MY_ASSERT(o->segid == seg, "Bad segid field after split\n");
  MY_ASSERT(o->address == (t_vaddr)right, "Bad address field after split\n");
  MY_ASSERT(o->offset == 4 * PAGESZ, "Bad offset field after split\n");
  MY_ASSERT(o->size == PAGESZ, "Bad size field after split\n");

  MY_ASSERT(region_coalesce(as, left, right, &reg) == ERROR_NONE,
	    "error coalescing regions\n");

  MY_ASSERT(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region after coalescing\n");

  MY_ASSERT(o->regid == reg, "Bad regid field after coalescing\n");
  MY_ASSERT(o->segid == seg, "Bad segid field after coalescing\n");
  MY_ASSERT(o->address == (t_vaddr)reg, "Bad address field after coalescing\n");
  MY_ASSERT(o->offset == PAGESZ, "Bad offset field after coalescing\n");
  MY_ASSERT(o->size == 4 * PAGESZ, "Bad size field after coalescing\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
