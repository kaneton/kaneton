#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid kasid;

void		check_region_multiple_resize_01(void)
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

  ASSERT(region_resize(kasid, reg, PAGESZ, &reg) == ERROR_NONE,
	    "error resizing region\n");

  ASSERT(region_get(kasid, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  ASSERT(o->regid == reg, "Bad regid field\n");
  ASSERT(o->segid == seg, "Bad segid field\n");
  ASSERT(o->address == (t_vaddr)reg, "Bad address field\n");
  ASSERT(o->offset == PAGESZ, "Bad offset field\n");
  ASSERT(o->size == PAGESZ, "Bad size field\n");

  ASSERT(region_resize(kasid, reg, 3 * PAGESZ, &reg) == ERROR_NONE,
	    "error resizing region\n");

  ASSERT(region_get(kasid, reg, &o) == ERROR_NONE,
	    "error getting region after resize\n");

  ASSERT(o->regid == reg, "Bad regid field after resize\n");
  ASSERT(o->segid == seg, "Bad segid field after resize\n");
  ASSERT(o->address == (t_vaddr)reg, "Bad address field after resize\n");
  ASSERT(o->offset == PAGESZ, "Bad offset field after resize\n");
  ASSERT(o->size == 3 * PAGESZ, "Bad size field after resize\n");

  t_uint8* p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + 3 * PAGESZ; p++)
    {
      *p = 0x0d;
      ASSERT(*p == 0x0d, "integrity check failed after resize\n");
    }

  ASSERT(region_release(kasid, reg) == ERROR_NONE,
	    "failed to release region\n");

  ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release region\n");

  TEST_LEAVE();
}
