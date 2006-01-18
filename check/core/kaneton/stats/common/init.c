/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/stats/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:20:27 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "init.h"

void	check_stats_init(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
  printf("init done.\n");
}

/*
 * starts ALL tests related to stats manager
 */

void	check_stats_tests(void)
{
  check_stats_init();
  check_stats_01();
  check_stats_02();
  check_stats_03();
  /* XXX continue tests */
}
