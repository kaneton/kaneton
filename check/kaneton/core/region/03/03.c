/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/03/03.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [mon jun 12 09:31:10 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid	kasid;

void		check_region_03(void)
{
  t_segid	seg;
  t_regid	reg;

  TEST_ENTER();

  ASSERT(segment_reserve(kasid,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  ASSERT(region_reserve(kasid,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   PAGESZ,
			   &reg) == ERROR_NONE, "error region_reserve\n");

  t_uint8* p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + PAGESZ; p++)
    *p = *p;

  ASSERT(region_release(kasid, reg) == ERROR_NONE,
	    "failed to release region\n");

  ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release region\n");

  TEST_LEAVE();
}
