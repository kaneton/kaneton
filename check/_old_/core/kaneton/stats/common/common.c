/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/stats/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [wed jan 18 18:55:11 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_stats_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to stats manager
 */

void	check_stats_tests(void)
{
  check_stats_common();
  check_stats_01();
  check_stats_02();
  check_stats_03();
  /* XXX continue tests */
}
