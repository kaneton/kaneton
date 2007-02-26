/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check-students/arch/ia32-virtual/kaneton/segment/multiple_reserve_03.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       julian pidancet   [sat feb 17 17:59:59 2007]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_multiple_reserve_03(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg[64];
  t_uint32	i;

  TEST_ENTER();

  TEST_NEW_AS(task, as);

  for (i = 0; i < 64; i++)
  {
    RESERVE_AND_CHECK(as, 2 * (i + 1), seg + i);
  }

  for (i = 0; i < 64; i++)
  {
    ASSERT(segment_release(seg[i]) == ERROR_NONE, "error releasing segment\n");
  }

  RESERVE_AND_CHECK(as, 3, seg);
  RESERVE_AND_CHECK(as, 9, seg + 1);
  RESERVE_AND_CHECK(as, 1, seg + 2);
  ASSERT(segment_release(seg[1]) == ERROR_NONE, "error releasing segment\n");
  RESERVE_AND_CHECK(as, 5, seg + 1);
  ASSERT(segment_release(seg[0]) == ERROR_NONE, "error releasing segment\n");
  RESERVE_AND_CHECK(as, 7, seg);
  RESERVE_AND_CHECK(as, 2, seg + 3);
  ASSERT(segment_release(seg[0]) == ERROR_NONE, "error releasing segment\n");
  ASSERT(segment_release(seg[1]) == ERROR_NONE, "error releasing segment\n");
  ASSERT(segment_release(seg[2]) == ERROR_NONE, "error releasing segment\n");
  ASSERT(segment_release(seg[3]) == ERROR_NONE, "error releasing segment\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
