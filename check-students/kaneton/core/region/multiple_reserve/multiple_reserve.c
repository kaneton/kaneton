#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid kasid;

void		check_region_multiple_reserve(void)
{
  t_segid	seg;
  t_regid	reg1;
  t_regid	reg2;
  t_regid	reg3;

  TEST_ENTER();

  ASSERT(segment_reserve(kasid,
			    10 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  ASSERT(region_reserve(kasid,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg1) == ERROR_NONE, "error region_reserve 1\n");
  ASSERT(region_reserve(kasid,
			   seg,
			   3 * PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg2) == ERROR_NONE, "error region_reserve 2\n");
  ASSERT(region_reserve(kasid,
			   seg,
			   6 * PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   4 * PAGESZ,
			   &reg3) == ERROR_NONE, "error region_reserve 3\n");

  t_uint8* p = (t_uint8*)(t_vaddr)reg1;
  for (; p < (t_uint8*)(t_vaddr)reg1 + 2 * PAGESZ; p++)
    *p = *p;
  p = (t_uint8*)(t_vaddr)reg2;
  for (; p < (t_uint8*)(t_vaddr)reg2 + 2 * PAGESZ; p++)
    *p = *p;
  p = (t_uint8*)(t_vaddr)reg3;
  for (; p < (t_uint8*)(t_vaddr)reg3 + 4 * PAGESZ; p++)
    *p = *p;

  ASSERT(region_release(kasid, reg1) == ERROR_NONE,
	    "failed to release region\n");
  ASSERT(region_release(kasid, reg2) == ERROR_NONE,
	    "failed to release region\n");
  ASSERT(region_release(kasid, reg3) == ERROR_NONE,
	    "failed to release region\n");

  ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release region\n");

  TEST_LEAVE();
}
