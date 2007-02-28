#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid kasid;

void		check_region_simple_get(void)
{
 i_segment	seg;
  i_region	reg;
  o_region*	o;

  TEST_ENTER();

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

  ASSERT(o->regid == reg, "Bad regid field\n");
  ASSERT(o->segid == seg, "Bad segid field\n");
  ASSERT(o->address == (t_vaddr)reg, "Bad address field\n");
  ASSERT(o->offset == PAGESZ, "Bad offset field\n");
  ASSERT(o->size == 2 * PAGESZ, "Bad size field\n");


  ASSERT(region_release(kasid, reg) == ERROR_NONE,
	    "failed to release region\n");

  ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release region\n");

  TEST_LEAVE();
}
