/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/stats/02/02.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:20:43 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../init/init.h"

/*
 * many nested calls
 */

void		check_stats_02(void)
{
  t_staid	st6;

  TEST_ENTER;

  if (STATS_RESERVE("stats_test_lotoffun", &st6) != ERROR_NONE)
    cons_msg('!', "error reserving stats objects\n");

  stats_begin(st6, "fun1");
  stats_begin(st6, "fun2");
  stats_begin(st6, "fun3");
  stats_begin(st6, "fun4");
  stats_begin(st6, "fun5");
  stats_begin(st6, "fun6");
  stats_end(st6, "fun1", ERROR_NONE);
  stats_end(st6, "fun2", ERROR_NONE);
  stats_end(st6, "fun3", ERROR_UNKNOWN);
  stats_end(st6, "fun4", ERROR_NONE);
  stats_end(st6, "fun5", ERROR_UNKNOWN);
  stats_end(st6, "fun6", ERROR_NONE);

  STATS_DUMP();

  if (STATS_RELEASE(st6) != ERROR_NONE)
    cons_msg('!', "error releasing stats objects\n");

  st6 = st6;

  TEST_LEAVE;
}
