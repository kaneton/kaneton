/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/region/05/05.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [thu sep  7 23:31:55 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid	kasid;

void		check_region_05(void)
{
  i_segment	seg;
  i_region	reg;
  o_region*	o;

  TEST_ENTER();

  MY_ASSERT(segment_reserve(kasid,
			    10 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  MY_ASSERT(region_reserve(kasid,
			   seg,
			   PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg) == ERROR_NONE, "error region_reserve\n");

  MY_ASSERT(region_resize(kasid, reg, PAGESZ, &reg) == ERROR_NONE,
	    "error resizing region\n");

  MY_ASSERT(region_get(kasid, reg, &o) == ERROR_NONE,
	    "error getting region\n");

  MY_ASSERT(o->regid == reg, "Bad regid field\n");
  MY_ASSERT(o->segid == seg, "Bad segid field\n");
  MY_ASSERT(o->address == (t_vaddr)reg, "Bad address field\n");
  MY_ASSERT(o->offset == PAGESZ, "Bad offset field\n");
  MY_ASSERT(o->size == PAGESZ, "Bad size field\n");

  MY_ASSERT(region_resize(kasid, reg, 3 * PAGESZ, &reg) == ERROR_NONE,
	    "error resizing region\n");

  MY_ASSERT(region_get(kasid, reg, &o) == ERROR_NONE,
	    "error getting region after resize\n");

  MY_ASSERT(o->regid == reg, "Bad regid field after resize\n");
  MY_ASSERT(o->segid == seg, "Bad segid field after resize\n");
  MY_ASSERT(o->address == (t_vaddr)reg, "Bad address field after resize\n");
  MY_ASSERT(o->offset == PAGESZ, "Bad offset field after resize\n");
  MY_ASSERT(o->size == 3 * PAGESZ, "Bad size field after resize\n");

  t_uint8* p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + 3 * PAGESZ; p++)
    {
      *p = 0x0d;
      MY_ASSERT(*p == 0x0d, "integrity check failed after resize\n");
    }

  MY_ASSERT(region_resize(kasid, reg, 20 * PAGESZ, &reg) != ERROR_NONE,
	    "resized a region to a bad size !\n");

  MY_ASSERT(region_release(kasid, reg) == ERROR_NONE,
	    "failed to release region\n");

  MY_ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release region\n");

  TEST_LEAVE();
}
