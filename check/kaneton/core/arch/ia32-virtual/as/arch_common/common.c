/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/arch/ia32-virtual/as/arch_common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [thu jul 20 19:43:24 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_as_arch_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to as manager
 */

void	check_as_arch_tests(void)
{
  check_as_arch_common();
  check_as_arch_01();
  check_as_arch_02();
  /* XXX continue tests */
}
