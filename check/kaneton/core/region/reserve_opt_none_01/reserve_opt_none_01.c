#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_reserve_opt_none_01(void)
{
  i_task	task;
  i_as		as;
  i_segment	seg;
  i_region	reg;
  o_region*	o;

  TEST_ENTER();

  TEST_NEW_AS(task, as);

  assert(segment_reserve(as,
			    2 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  assert(region_reserve(as,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  assert(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  assert(o->regid == reg, "Bad regid field\n");
  assert(o->segid == seg, "Bad segid field\n");
  assert(o->address == (t_vaddr)reg, "Bad address field\n");
  assert(o->offset == 0, "Bad offset field\n");
  assert(o->size == 2 * PAGESZ, "Bad size field\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
