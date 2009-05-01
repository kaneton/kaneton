#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern i_as	kasid;

void		check_region_simple_get_01(void)
{
  i_segment	seg;
  i_region	reg;
  o_region*	o;

  TEST_ENTER();

  assert(segment_reserve(kasid,
			    10 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  assert(region_reserve(kasid,
			   seg,
			   PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg) == ERROR_NONE, "error region_reserve\n");

  assert(region_get(kasid, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  assert(o->regid == reg, "Bad regid field\n");
  assert(o->segid == seg, "Bad segid field\n");
  assert(o->address == (t_vaddr)reg, "Bad address field\n");
  assert(o->offset == PAGESZ, "Bad offset field\n");
  assert(o->size == 2 * PAGESZ, "Bad size field\n");

  TEST_LEAVE();
}
