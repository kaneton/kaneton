#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern i_as kasid;

void		check_region_split_coalesce_01(void)
{
   i_task	task;
  i_as	as;
  i_segment	seg;
  i_region	reg;
  o_region*	o;
  i_region	left;
  i_region	right;

  TEST_ENTER();

  assert(task_reserve(TASK_CLASS_GUEST,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  assert(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  assert(segment_reserve(as,
			    5 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  assert(region_reserve(as,
			   seg,
			   PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   4 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  assert(region_split(as, reg, 3 * PAGESZ, &left, &right) == ERROR_NONE,
	    "error splitting region\n");

  assert(region_get(as, left, &o) == ERROR_NONE,
	    "error getting region after split\n");

  assert(o->regid == left, "Bad regid field after split\n");
  assert(o->segid == seg, "Bad segid field after split\n");
  assert(o->address == (t_vaddr)left, "Bad address field after split\n");
  assert(o->offset == PAGESZ, "Bad offset field after split\n");
  assert(o->size == 3 * PAGESZ, "Bad size field after split\n");

  assert(region_get(as, right, &o) == ERROR_NONE,
	    "error getting region after split\n");

  assert(o->regid == right, "Bad regid field after split\n");
  assert(o->segid == seg, "Bad segid field after split\n");
  assert(o->address == (t_vaddr)right, "Bad address field after split\n");
  assert(o->offset == 4 * PAGESZ, "Bad offset field after split\n");
  assert(o->size == PAGESZ, "Bad size field after split\n");

  assert(region_coalesce(as, left, right, &reg) == ERROR_NONE,
	    "error coalescing regions\n");

  assert(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region after coalescing\n");

  assert(o->regid == reg, "Bad regid field after coalescing\n");
  assert(o->segid == seg, "Bad segid field after coalescing\n");
  assert(o->address == (t_vaddr)reg, "Bad address field after coalescing\n");
  assert(o->offset == PAGESZ, "Bad offset field after coalescing\n");
  assert(o->size == 4 * PAGESZ, "Bad size field after coalescing\n");

  assert(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
