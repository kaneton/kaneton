/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/stats/03/03.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [wed jan 18 18:56:52 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * errors tests
 */

void		check_stats_03(void)
{
  t_staid	sterr;

  TEST_ENTER;

  if (STATS_RESERVE("stats_test_lotoffun", &sterr) != ERROR_NONE)
    cons_msg('!', "error reserving stats objects\n");

  stats_end(sterr, "foo", ERROR_UNKNOWN);
  stats_begin(sterr, "foo");
  stats_end(sterr, "foo", ERROR_UNKNOWN);
  stats_end(sterr, "foo", ERROR_UNKNOWN);
  stats_begin(sterr, "foo");
  stats_begin(sterr, "foo");
  stats_begin(sterr, "foo");
  stats_end(sterr, "foo", ERROR_UNKNOWN);
  STATS_RELEASE(-4);
  STATS_RELEASE(-100);

  STATS_DUMP();

  if (STATS_RELEASE(sterr) != ERROR_NONE)
    cons_msg('!', "error releasing stats objects\n");

  sterr = sterr;

  TEST_LEAVE;
}
