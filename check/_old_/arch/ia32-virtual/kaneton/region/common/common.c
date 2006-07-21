/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [sat apr 15 23:01:14 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_region_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to region manager
 */

void	check_region_tests(void)
{
  check_region_common();
  check_region_01();
  check_region_02();
  check_region_03();
  /* XXX continue tests */
}
