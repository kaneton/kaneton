/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check-students/arch/ia32-virtual/kaneton/segment/simple_reserve_02.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       julian pidancet   [sat feb 17 17:59:59 2007]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * basic operations: reserve, get and release.
 */

void		check_segment_simple_reserve_02(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  o_segment*	o;

  TEST_ENTER();

  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as,
			    12 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment\n");

  ASSERT(o->segid == seg, "Bad segid field\n");
  ASSERT(o->asid == as, "Bad asid field\n");
  ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field\n");
  ASSERT(o->address == (t_uint32)seg, "Bad address field\n");
  ASSERT(o->size == 12 * PAGESZ, "Bad size field\n");
  ASSERT(o->perms == PERM_READ, "Bad perms field\n");

  ASSERT(segment_release(seg) == ERROR_NONE, "error releasing segment\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
