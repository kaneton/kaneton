#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid kasid;

void		check_region_resize_error_01(void)
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

  ASSERT(region_resize(kasid, reg, 20 * PAGESZ, &reg) != ERROR_NONE,
	    "resized a region to a bad size !\n");

  ASSERT(region_release(kasid, reg) == ERROR_NONE,
	    "failed to release region\n");

  ASSERT(segment_release(seg) == ERROR_NONE,
	    "failed to release region\n");

  TEST_LEAVE();
}
