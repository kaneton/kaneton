#include <klibc.h>
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

  ASSERT(segment_reserve(as,
			    2 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  ASSERT(region_reserve(as,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  ASSERT(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  ASSERT(o->regid == reg, "Bad regid field\n");
  ASSERT(o->segid == seg, "Bad segid field\n");
  ASSERT(o->address == (t_vaddr)reg, "Bad address field\n");
  ASSERT(o->offset == 0, "Bad offset field\n");
  ASSERT(o->size == 2 * PAGESZ, "Bad size field\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}