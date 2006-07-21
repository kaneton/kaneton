/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/12/12.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun apr  9 19:37:50 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid	kasid;

/*
 * resize.
 */

void		check_segment_12(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_segid	seg2;
  t_segid	seg3;
  t_uint32	try = 0;
  o_segment*	o;
  t_regid	reg;
  t_uint32	i;
  t_uint8*	buff;

  TEST_ENTER;
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  while (try < 40)
    {
      MY_ASSERT(segment_reserve(as,
				PAGESZ,
				PERM_READ,
				&seg2) == ERROR_NONE,
		"error reserving segment\n");

      if (seg2 == seg + PAGESZ)
	break;

      seg = seg2;

      try++;
    }

  if (try == 40)
    MY_ASSERT(0, "FATAL ERROR\n");

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

  MY_ASSERT(region_release(kasid, reg) == ERROR_NONE,
	    "error releasing region\n");

  MY_ASSERT(segment_resize(seg, 3 * PAGESZ, &seg3) == ERROR_NONE,
	    "error resizing\n");

  MY_ASSERT(seg3 != seg, "some segments are overlapping\n");

  seg = seg3;

  MY_ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment\n");

  MY_ASSERT(o->segid == seg, "Bad segid field\n");
  MY_ASSERT(o->asid == as, "Bad asid field\n");
  MY_ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field\n");
  MY_ASSERT(o->address == (t_uint32)seg, "Bad address field\n");
  MY_ASSERT(o->size == 3 * PAGESZ, "Bad size field\n");
  MY_ASSERT(o->perms == PERM_READ, "Bad perms field\n");

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
      MY_ASSERT(buff[i] == (i * 2 + 4) % 256, "data badly copied\n");
    }

  MY_ASSERT(region_release(kasid, reg) == ERROR_NONE,
	    "error releasing region\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
