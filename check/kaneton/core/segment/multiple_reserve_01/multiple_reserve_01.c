/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/2008-K1/kaneton/check/kaneton/core/segment/multiple_reserve_01/multiple_reserve_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sat mar  3 19:05:33 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_multiple_reserve_01(void)
{
  i_task	task;
  i_as		as;
  i_segment	seg[4];

  TEST_ENTER();

  TEST_NEW_AS(task, as);

  RESERVE_AND_CHECK(as, 2, seg);
  RESERVE_AND_CHECK(as, 8, seg + 1);
  RESERVE_AND_CHECK(as, 2, seg + 2);
  RESERVE_AND_CHECK(as, 5, seg + 1);
  RESERVE_AND_CHECK(as, 8, seg);
  RESERVE_AND_CHECK(as, 4, seg + 3);

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
