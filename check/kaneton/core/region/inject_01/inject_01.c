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

  TEST_ENTER();
  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(kasid,
			    10 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  o = malloc(sizeof (o_region));
  o->segid = seg;
  o->address = 0x40000000;
  o->offset = PAGESZ;
  o->size = 2 * PAGESZ;
  o->opts = REGION_OPT_FORCE;

  ASSERT(ia32_map_region(as, seg,
			 PAGESZ, REGION_OPT_FORCE,
			 0x40000000, 2 * PAGESZ) == ERROR_NONE,
	 "error mapping the region");

  ASSERT(region_inject(as, o, &reg) == ERROR_NONE, "error injecting region\n");

  ASSERT(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  ASSERT(o->regid == reg, "Bad regid field\n");
  ASSERT(o->segid == seg, "Bad segid field\n");
  ASSERT(o->address == (t_vaddr)reg, "Bad address field\n");
  ASSERT(o->offset == PAGESZ, "Bad offset field\n");
  ASSERT(o->size == 2 * PAGESZ, "Bad size field\n");
  ASSERT(o->opts == REGION_OPT_FORCE, "Bad opts field\n");

  ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release segment\n");

  TEST_LEAVE_AS(task, as);
  TEST_LEAVE();
}
