#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern i_as kasid;

void		check_region_inject_01(void)
{
  i_as	as;
  i_task	task;
  i_segment	seg;
  i_region	reg;
  o_region*	o;
  o_region*	o_dup;

  TEST_ENTER();
  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(kasid,
			    10 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  ASSERT(region_reserve(kasid,
			   seg,
			   PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg) == ERROR_NONE, "error region_reserve\n");

  ASSERT(region_get(kasid, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  o_dup = malloc(sizeof (o_region));
  memcpy(o_dup, o, sizeof (o_region));

  ASSERT(region_inject(as, o_dup, &reg) == ERROR_NONE, "error injecting region\n");

  region_dump(as);

  ASSERT(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  ASSERT(o->regid == reg, "Bad regid field\n");
  ASSERT(o->segid == seg, "Bad segid field\n");
  ASSERT(o->address == (t_vaddr)reg, "Bad address field\n");
  ASSERT(o->offset == PAGESZ, "Bad offset field\n");
  ASSERT(o->size == 2 * PAGESZ, "Bad size field\n");


  ASSERT(region_release(kasid, reg) == ERROR_NONE,
	    "failed to release kernel region\n");

  ASSERT(region_release(as, reg) == ERROR_NONE,
	    "failed to release region\n");

  ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release segment\n");

  TEST_LEAVE_AS(task, as);
  TEST_LEAVE();
}
