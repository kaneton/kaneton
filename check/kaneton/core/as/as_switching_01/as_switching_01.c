#include <libc/libc.h>
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

  assert(task_reserve(TASK_CLASS_PROGRAM,
		      TASK_BEHAV_INTERACTIVE,
		      TASK_PRIOR_INTERACTIVE,
		      &task) == ERROR_NONE,
	 "Error creating task\n");
  assert(as_reserve(task, &as) == ERROR_NONE,
	 "Error creating as\n");

  assert(as_get(as, &o) == ERROR_NONE,
	 "Error in as_get\n");

  assert(region_reserve(as,
			(i_segment)init->kstack,
			0,
			REGION_OPT_FORCE,
			(t_vaddr)init->kstack,
			init->kstacksz,
			&reg) == ERROR_NONE,
	 "Error reserving region for kstack\n");

  assert(segment_reserve(as,
			 PAGESZ,
			 PERM_READ | PERM_WRITE,
			 &seg) == ERROR_NONE,
	 "Error in segment_reserve");

  assert(region_reserve(as,
			seg,
			0,
			REGION_OPT_NONE,
			0,
			PAGESZ,
			&reg)  == ERROR_NONE,
	 "Error in region_reserve");

  assert(map_reserve(as,
		     MAP_OPT_NONE,
		     PAGESZ,
		     PERM_READ | PERM_WRITE,
		     &addr) == ERROR_NONE,
	 "Error in map_reserve");

  assert(as_get(kasid, &o) == ERROR_NONE,
	    "cannot get kernel as\n");

  kdir = o->machdep.pd;

  assert(as_get(as, &o) == ERROR_NONE,
	    "cannot get created as\n");

  ptr1 = (int*)(t_uint32)reg;
  ptr2 = (int*)addr;

  assert(ia32_pd_activate(o->machdep.pd, IA32_PD_CACHED,
			  IA32_PD_WRITEBACK) == ERROR_NONE,
	    "cannot switch as\n");

  *ptr1 = 0x41424344;
  i = *ptr1;
  *ptr2 = 0x40414243;
  j = *ptr2;

  ia32_pd_activate(kdir, IA32_PD_CACHED, IA32_PD_WRITEBACK);

  assert(i == 0x41424344, "read/write test failed\n");

  assert(j == 0x40414243, "read/write test failed\n");

  assert(as_release(as) == ERROR_NONE,
	 "Failed to release as\n");
  assert(task_release(task) == ERROR_NONE,
	 "Failed to release task\n");

  TEST_LEAVE();
}
