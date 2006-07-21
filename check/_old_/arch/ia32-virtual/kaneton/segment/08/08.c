/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/08/08.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun mar 26 23:01:44 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid	kasid;

/*
 * segment clone
 */

void		check_segment_08(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_regid	reg;
  o_segment*	o;
  t_uint8*	buff;
  t_uint32	i;

  TEST_ENTER;
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");
  MY_ASSERT(region_reserve(kasid,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  buff = (t_uint8*)(t_uint32)reg;
  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  MY_ASSERT(segment_write(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error writing to segment\n");

  MY_ASSERT(segment_clone(as, seg, &seg) == ERROR_NONE,
	    "error cloning segment\n");

  MY_ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment\n");

  MY_ASSERT(o->asid == as, "Bad as field\n");
  MY_ASSERT(o->segid == seg, "Bad segid field\n");
  MY_ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field\n");
  MY_ASSERT(o->address == (t_paddr)seg, "Bad address field\n");
  MY_ASSERT(o->perms == (PERM_READ | PERM_WRITE), "Bad as field\n");
  MY_ASSERT(o->size == PAGESZ, "Bad as field\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  MY_ASSERT(segment_read(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error reading to segment\n");

  for (i = 0; i < PAGESZ; i++)
    {
      MY_ASSERT(buff[i] == (i * 2 + 4) % 256, "bad data\n");
    }

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
