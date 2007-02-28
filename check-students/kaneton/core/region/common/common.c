/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [thu jul 20 19:07:45 2006]
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
  check_region_reserve_opt_none();
  check_region_reserve_opt_force();
  check_region_multiple_flush();
  check_region_simple_flush();
  check_region_simple_write();
}
