#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;

void		check_as_as_switching_01(void)
{
  i_task	task;
  i_as		as;
  int		i, j;
  i_segment	seg;
  i_region	reg;
  t_vaddr	addr;
  o_as*		o;
  t_ia32_directory	kdir;
  int*		ptr1;
  int*		ptr2;

  TEST_ENTER();

  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
		      TASK_BEHAV_INTERACTIVE,
		      TASK_PRIOR_INTERACTIVE,
		      &task) == ERROR_NONE,
	 "Error creating task\n");
  ASSERT(as_reserve(task, &as) == ERROR_NONE,
	 "Error creating as\n");

  ASSERT(as_get(as, &o) == ERROR_NONE,
	 "Error in as_get\n");

  ASSERT(region_reserve(as,
			(t_segid)init->kstack,
			0,
			REGION_OPT_FORCE,
			(t_vaddr)init->kstack,
			init->kstacksz,
			&reg) == ERROR_NONE,
	 "Error reserving region for kstack\n");

  ASSERT(segment_reserve(as,
			 PAGESZ,
			 PERM_READ | PERM_WRITE,
			 &seg) == ERROR_NONE,
	 "Error in segment_reserve");

  ASSERT(region_reserve(as,
			seg,
			0,
			REGION_OPT_NONE,
			0,
			PAGESZ,
			&reg)  == ERROR_NONE,
	 "Error in region_reserve");

  ASSERT(map_reserve(as,
		     MAP_OPT_NONE,
		     PAGESZ,
		     PERM_READ | PERM_WRITE,
		     &addr) == ERROR_NONE,
	 "Error in map_reserve");

  ASSERT(as_get(kasid, &o) == ERROR_NONE,
	    "cannot get kernel as\n");

  kdir = o->machdep.pd;

  ASSERT(as_get(as, &o) == ERROR_NONE,
	    "cannot get created as\n");

  ptr1 = (int*)(t_uint32)reg;
  ptr2 = (int*)addr;

  ASSERT(pd_activate(o->machdep.pd, PD_CACHED, PD_WRITEBACK) == ERROR_NONE,
	    "cannot switch as\n");

  *ptr1 = 0x41424344;
  i = *ptr1;
  *ptr2 = 0x40414243;
  j = *ptr2;

  pd_activate(kdir, PD_CACHED, PD_WRITEBACK);

  ASSERT(i == 0x41424344, "read/write test failed\n");

  ASSERT(j == 0x40414243, "read/write test failed\n");

  ASSERT(as_release(as) == ERROR_NONE,
	 "Failed to release as\n");
  ASSERT(task_release(task) == ERROR_NONE,
	 "Failed to release task\n");

  TEST_LEAVE();
}
