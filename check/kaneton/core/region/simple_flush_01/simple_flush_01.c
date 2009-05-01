#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_simple_flush_01(void)
{
  i_task	task;
  i_as		as;
  i_segment	seg;
  i_region	reg;
  t_uint32	i;
  o_as*		o;
  t_setsz	sz;

  TEST_ENTER();

  TEST_NEW_AS(task, as);

  assert(as_get(as, &o) == ERROR_NONE, "error getting as\n");

  assert(segment_reserve(as,
			 PAGESZ,
			 PERM_READ | PERM_WRITE,
			 &seg) == ERROR_NONE,
	 "error reserving segment\n");

  assert(region_reserve(as,
			seg,
			0,
			REGION_OPT_NONE,
			0,
			PAGESZ,
			&reg) == ERROR_NONE,
	 "error reserving region\n");

  assert(region_flush(as) == ERROR_NONE, "error flushing regions\n");

  assert(set_size(o->regions, &sz) == ERROR_NONE, "error in get size\n");

  assert(sz == 0, "wrong size\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
