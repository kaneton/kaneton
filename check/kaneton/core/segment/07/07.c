/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/07/07.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun apr  9 18:16:18 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid kasid;

/*
 * direct r/w + copy
 */

void		check_segment_07(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_segid	seg_ref;
  t_regid	reg;
  t_uint32	i;
  t_uint8*	buff;

  TEST_ENTER();
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg_ref) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(region_reserve(kasid,
			   seg_ref,
			   0,
			   REGION_OPT_NONE,
			   0,
			   PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  /* R/W */

  buff = (t_uint8*)(t_uint32)reg;
  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  MY_ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(segment_write(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error writing\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  MY_ASSERT(segment_read(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error reading\n");

  for (i = 0; i < PAGESZ; i++)
    {
      MY_ASSERT(buff[i] == (i * 2 + 4) % 256, "bad data\n");
    }

  i = 0x41424344;
  MY_ASSERT(segment_write(seg, 44, &i, sizeof (t_uint32)) == ERROR_NONE,
	    "error writing\n");

  i = 0;
  MY_ASSERT(segment_read(seg, 44, &i, sizeof (t_uint32)) == ERROR_NONE,
	    "error reading\n");

  MY_ASSERT(i == 0x41424344, "bad data\n");

  /* copy */

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  MY_ASSERT(segment_copy(seg, 0, seg_ref, 0,
			 PAGESZ) == ERROR_NONE,
	    "error in copy\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  MY_ASSERT(segment_read(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error reading\n");

  for (i = 0; i < PAGESZ; i++)
    {
      MY_ASSERT(buff[i] == (i * 4 - 1) % 256, "bad data\n");
    }

  /* Errors */

  MY_ASSERT(segment_perms(seg, PERM_READ) == ERROR_NONE,
	    "error setting perms\n");

  MY_ASSERT(segment_write(seg, 0, buff, sizeof(t_uint32)) != ERROR_NONE,
	    "error: wrote to ro segment\n");

  MY_ASSERT(segment_copy(seg, 0, seg_ref, 0,
			 sizeof(t_uint32)) != ERROR_NONE,
	    "error: copy to ro segment\n");

  MY_ASSERT(segment_read(seg, 2 * PAGESZ, buff,
			 sizeof(t_uint32)) != ERROR_NONE,
	    "error: out of bound reading\n");

  MY_ASSERT(segment_read(seg, 0, buff,
			 2 * PAGESZ) != ERROR_NONE,
	    "error: size overflow reading\n");

  MY_ASSERT(segment_write(seg, 2 * PAGESZ, buff,
			 sizeof(t_uint32)) != ERROR_NONE,
	    "error: out of bound writing\n");

  MY_ASSERT(segment_write(seg, 0, buff,
			 2 * PAGESZ) != ERROR_NONE,
	    "error: size overflow writing\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
