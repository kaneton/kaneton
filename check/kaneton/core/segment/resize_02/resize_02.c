/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/resize_02/resize_02.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:48:35 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern i_as	kasid;

/*
 * resize.
 */

void		check_segment_resize_02(void)
{
  i_task	task;
  i_as	as;
  i_segment	seg;
  i_segment	seg2;
  i_segment	seg3;
  t_uint32	try = 0;
  o_segment*	o;
  i_region	reg;
  t_uint32	i;
  t_uint8*	buff;

  TEST_ENTER();
  assert(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  assert(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  assert(segment_reserve(as,
			    PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  while (try < 40)
    {
      assert(segment_reserve(as,
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
    assert(0, "FATAL ERROR\n");

  assert(region_reserve(kasid,
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

  assert(region_release(kasid, reg) == ERROR_NONE,
	    "error releasing region\n");

  assert(segment_resize(seg, 3 * PAGESZ, &seg3) == ERROR_NONE,
	    "error resizing\n");

  assert(seg3 != seg, "some segments are overlapping after resize\n");

  seg = seg3;

  assert(segment_get(seg, &o) == ERROR_NONE, "error getting segment after resize\n");

  assert(o->segid == seg, "Bad segid field after resize\n");
  assert(o->asid == as, "Bad asid field after resize\n");
  assert(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after resize\n");
  assert(o->address == (t_uint32)seg, "Bad address field after resize\n");
  assert(o->size == 3 * PAGESZ, "Bad size field after resize\n");
  assert(o->perms == PERM_READ, "Bad perms field after resize\n");

  assert(region_reserve(kasid,
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
      assert(buff[i] == (i * 2 + 4) % 256, "data badly copied after resize\n");
    }

  assert(region_release(kasid, reg) == ERROR_NONE,
	    "error releasing region\n");

  assert(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
