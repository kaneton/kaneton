/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/map/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [thu jun  1 18:14:18 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_map_01(void)
{
  i_task	task;
  t_asid	as;
  t_vaddr	addr;
  t_uint32	i;
  t_uint32	j;
  t_uint8*	p;

  TEST_ENTER;

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");


  for (i = 0; i < 200; i++)
    {
      MY_ASSERT(map_reserve(as,
			    MAP_OPT_NONE,
			    ((i % 100) + 1) * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &addr) == ERROR_NONE,
		"error reserving a map\n");

      for (j = 0, p = (t_uint8*)addr;
	   j < ((i % 100) + 1) * PAGESZ;
	   p++, j++)
	{
	  *p = 0x0d;
	  MY_ASSERT(*p == 0x0d, "integrity check failed\n");
	}

      MY_ASSERT(map_release(as, addr) == ERROR_NONE,
		"error releasing a map\n");
    }

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
