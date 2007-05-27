#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_multiple_flush_01(void)
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

  ASSERT(as_get(as, &o) == ERROR_NONE, "error getting as\n");

  for (i = 0; i < 10; i++)
    {
      ASSERT(segment_reserve(as,
				PAGESZ,
				PERM_READ | PERM_WRITE,
				&seg) == ERROR_NONE,
		"error reserving segment\n");

      ASSERT(region_reserve(as,
			       seg,
			       0,
			       REGION_OPT_NONE,
			       0,
			       PAGESZ,
			       &reg) == ERROR_NONE,
		"error reserving region\n");
    }

  ASSERT(region_flush(as) == ERROR_NONE, "error flushing regions\n");

  ASSERT(set_size(o->regions, &sz) == ERROR_NONE, "error in get size\n");

  ASSERT(sz == 0, "wrong size\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
