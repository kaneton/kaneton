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

  assert(segment_reserve(kasid,
			    10 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  o = malloc(sizeof (o_region));
  o->segid = seg;
  o->address = 0x40000000;
  o->offset = PAGESZ;
  o->size = 2 * PAGESZ;
  o->opts = REGION_OPT_FORCE;

  assert(ia32_map_region(as, seg,
			 PAGESZ, REGION_OPT_FORCE,
			 0x40000000, 2 * PAGESZ) == ERROR_NONE,
	 "error mapping the region");

  assert(region_inject(as, o, &reg) == ERROR_NONE, "error injecting region\n");

  assert(region_get(as, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  assert(o->regid == reg, "Bad regid field\n");
  assert(o->segid == seg, "Bad segid field\n");
  assert(o->address == (t_vaddr)reg, "Bad address field\n");
  assert(o->offset == PAGESZ, "Bad offset field\n");
  assert(o->size == 2 * PAGESZ, "Bad size field\n");
  assert(o->opts == REGION_OPT_FORCE, "Bad opts field\n");

  assert(segment_release(seg) == ERROR_NONE,
	    "failed to release segment\n");

  TEST_LEAVE_AS(task, as);
  TEST_LEAVE();
}
