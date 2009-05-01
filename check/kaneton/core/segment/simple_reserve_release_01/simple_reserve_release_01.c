/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/2008-K1/kaneton/check/kaneton/core/segment/simple_reserve_release_01/simple_reserve_release_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sat mar  3 19:02:52 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * basic operations: reserve, get and release.
 */

void		check_segment_simple_reserve_release_01(void)
{
  i_task	task;
  i_as		as;
  i_segment	seg;
  o_segment*	o;

  TEST_ENTER();

  TEST_NEW_AS(task, as);

  assert(segment_reserve(as,
			    2 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  assert(segment_get(seg, &o) == ERROR_NONE, "error getting segment\n");

  assert(o->segid == seg, "Bad segid field\n");
  assert(o->asid == as, "Bad asid field\n");
  assert(o->address == (t_uint32)seg, "Bad address field\n");
  assert(o->size == 2 * PAGESZ, "Bad size field\n");
  assert(o->perms == PERM_READ, "Bad perms field\n");

  assert(segment_release(seg)  == ERROR_NONE, "error releasing segment\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
