/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/04/04.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [fri jun  2 14:09:15 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * giving segments from one as to another.
 */

void		check_segment_04(void)
{
  i_task	task1;
  i_task	task2;
  t_asid	as1;
  t_asid	as2;
  t_segid	seg;
  o_segment*	o;
  o_as*		oas;
  void*		chiche;

  TEST_ENTER;
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task1) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task2) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task1, &as1) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(as_reserve(task2, &as2) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as1,
			    2 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(segment_give(as2, seg) == ERROR_NONE, "error giving segment\n");

  MY_ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment\n");

  MY_ASSERT(o->asid == as2, "Bad asid field\n");

  MY_ASSERT(as_get(as1, &oas) == ERROR_NONE, "error getting as\n");

  MY_ASSERT(set_get(oas->segments, seg, &chiche) != ERROR_NONE,
	    "error: segment not removed from source as\n");

  MY_ASSERT(as_get(as2, &oas) == ERROR_NONE, "error getting as\n");

  MY_ASSERT(set_get(oas->segments, seg, &chiche) == ERROR_NONE,
	    "error: segment not added to destination as\n");

  MY_ASSERT(as_release(as1) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(as_release(as2) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task1) == ERROR_NONE,
	    "failed to release task\n");

  MY_ASSERT(task_release(task2) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
