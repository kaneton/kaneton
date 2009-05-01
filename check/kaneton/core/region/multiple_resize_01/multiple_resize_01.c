#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern i_as kasid;

void		check_region_multiple_resize_01(void)
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

  assert(region_resize(kasid, reg, PAGESZ, &reg) == ERROR_NONE,
	    "error resizing region\n");

  assert(region_get(kasid, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  assert(o->regid == reg, "Bad regid field\n");
  assert(o->segid == seg, "Bad segid field\n");
  assert(o->address == (t_vaddr)reg, "Bad address field\n");
  assert(o->offset == PAGESZ, "Bad offset field\n");
  assert(o->size == PAGESZ, "Bad size field\n");

  assert(region_resize(kasid, reg, 3 * PAGESZ, &reg) == ERROR_NONE,
	    "error resizing region\n");

  assert(region_get(kasid, reg, &o) == ERROR_NONE,
	    "error getting region after resize\n");

  assert(o->regid == reg, "Bad regid field after resize\n");
  assert(o->segid == seg, "Bad segid field after resize\n");
  assert(o->address == (t_vaddr)reg, "Bad address field after resize\n");
  assert(o->offset == PAGESZ, "Bad offset field after resize\n");
  assert(o->size == 3 * PAGESZ, "Bad size field after resize\n");

  t_uint8* p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + 3 * PAGESZ; p++)
    {
      *p = 0x0d;
      assert(*p == 0x0d, "integrity check failed after resize\n");
    }

  assert(region_release(kasid, reg) == ERROR_NONE,
	    "failed to release region\n");

  assert(segment_release(seg) == ERROR_NONE,
	    "failed to release region\n");

  TEST_LEAVE();
}
