/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/04/04.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [mon jun 12 09:32:00 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid	kasid;

void		check_region_04(void)
{
  t_segid	seg;
  t_regid	reg1;
  t_regid	reg2;
  t_regid	reg3;

  TEST_ENTER;

  MY_ASSERT(segment_reserve(kasid,
			    10 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  MY_ASSERT(region_reserve(kasid,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg1) == ERROR_NONE, "error region_reserve\n");
  MY_ASSERT(region_reserve(kasid,
			   seg,
			   3 * PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg2) == ERROR_NONE, "error region_reserve\n");
  MY_ASSERT(region_reserve(kasid,
			   seg,
			   6 * PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   4 * PAGESZ,
			   &reg3) == ERROR_NONE, "error region_reserve\n");

  t_uint8* p = (t_uint8*)(t_vaddr)reg1;
  for (; p < (t_uint8*)(t_vaddr)reg1 + 2 * PAGESZ; p++)
    *p = *p;
  p = (t_uint8*)(t_vaddr)reg2;
  for (; p < (t_uint8*)(t_vaddr)reg2 + 2 * PAGESZ; p++)
    *p = *p;
  p = (t_uint8*)(t_vaddr)reg3;
  for (; p < (t_uint8*)(t_vaddr)reg3 + 4 * PAGESZ; p++)
    *p = *p;

  MY_ASSERT(region_release(kasid, reg1) == ERROR_NONE,
	    "failed to release region\n");
  MY_ASSERT(region_release(kasid, reg2) == ERROR_NONE,
	    "failed to release region\n");
  MY_ASSERT(region_release(kasid, reg3) == ERROR_NONE,
	    "failed to release region\n");

  MY_ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release region\n");

  TEST_LEAVE;
}
