/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/2008-K1/check/kaneton/check/kaneton/core/segment/multiple_reserve_03/multiple_reserve_03.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sat mar  3 21:24:24 2007]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_multiple_reserve_03(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg[64];
  t_uint32	i, j;

  TEST_ENTER();

  TEST_NEW_AS(task, as);

  for (i = 0; i < 64; i++)
  {
    RESERVE_AND_CHECK(as, (i % 10) + 1, seg + i);
    for (j = 0; j < 64; j++)
      if (i != j && seg[i] == seg[j])
	{
	  printf("error: some segment are overlapping");
	  TEST_LEAVE();
	}
  }

  RESERVE_AND_CHECK(as, 3, seg);
  RESERVE_AND_CHECK(as, 9, seg + 1);
  RESERVE_AND_CHECK(as, 1, seg + 2);
  RESERVE_AND_CHECK(as, 5, seg + 1);
  RESERVE_AND_CHECK(as, 7, seg);
  RESERVE_AND_CHECK(as, 2, seg + 3);

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
