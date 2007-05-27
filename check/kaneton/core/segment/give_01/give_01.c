/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/give_01/give_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:43:23 2007]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * giving segments from one as to another.
 */

void		check_segment_give_01(void)
{
  i_task	task1;
  i_task	task2;
  t_asid	as1;
  t_asid	as2;
  t_segid	seg;
  o_segment*	o;
  o_as*		oas;
  void*		chiche;

  TEST_ENTER();
  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task1) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task2) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(as_reserve(task1, &as1) == ERROR_NONE, "error creating as\n");

  ASSERT(as_reserve(task2, &as2) == ERROR_NONE, "error creating as\n");

  ASSERT(segment_reserve(as1,
			    2 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  ASSERT(segment_give(as2, seg) == ERROR_NONE, "error giving segment\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment after segment_give\n");

  ASSERT(o->asid == as2, "Bad asid field after segment_give\n");

  ASSERT(as_get(as1, &oas) == ERROR_NONE, "error getting as after segment_give\n");

  ASSERT(set_get(oas->segments, seg, &chiche) != ERROR_NONE,
	    "error: segment not removed from source as after segment_give\n");

  ASSERT(as_get(as2, &oas) == ERROR_NONE, "error getting as after segment_give\n");

  ASSERT(set_get(oas->segments, seg, &chiche) == ERROR_NONE,
	    "error: segment not added to destination as after segment_give\n");

  ASSERT(as_release(as1) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(as_release(as2) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task1) == ERROR_NONE,
	    "failed to release task\n");

  ASSERT(task_release(task2) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
