#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_as_with_map_01(void)
{
  i_task	task;
  i_as		as;
  int		i;
  t_vaddr	addr;
  t_setsz	sz1_before;
  t_setsz	sz1_after;
  t_setsz	sz2_before;
  t_setsz	sz2_after;
  o_as*		o;

  TEST_ENTER();

  assert(task_reserve(TASK_CLASS_PROGRAM,
		      TASK_BEHAV_INTERACTIVE,
		      TASK_PRIOR_INTERACTIVE,
		      &task) == ERROR_NONE,
	 "Error creating task\n");
  assert(as_reserve(task, &as) == ERROR_NONE,
	 "Error creating as\n");

  assert(as_get(as, &o) == ERROR_NONE,
	 "Error in as_get\n");

  assert(set_size(o->segments, &sz1_before) == ERROR_NONE,
	 "Error in set_size\n");
  assert(set_size(o->regions, &sz2_before) == ERROR_NONE,
	 "Error in set_size\n");

  for (i = 0; i < 10; i++)
    {
      assert(map_reserve(as,
			 MAP_OPT_NONE,
			 PAGESZ,
			 PERM_READ | PERM_WRITE,
			 &addr) == ERROR_NONE,
	     "Error in map_reserve");
    }

  assert(set_size(o->segments, &sz1_after) == ERROR_NONE,
	 "Error in set_size\n");
  assert(set_size(o->regions, &sz2_after) == ERROR_NONE,
	 "Error in set_size\n");

  assert(sz1_after >= sz1_before + 10,
	 "Error, segments of maps were not reserved correctly\n");
  assert(sz2_after >= sz2_before + 10,
	 "Error, regions of maps were not reserved correctly\n");

  assert(as_release(as) == ERROR_NONE,
	 "Failed to release as\n");
  assert(task_release(task) == ERROR_NONE,
	 "Failed to release task\n");

  TEST_LEAVE();
}
