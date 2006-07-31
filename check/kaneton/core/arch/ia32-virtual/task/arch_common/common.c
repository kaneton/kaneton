/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/arch/ia32-virtual/task/arch_common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [sun jul 30 18:21:41 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_task_arch_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to task manager
 */

void	check_task_arch_tests(void)
{
  check_task_arch_common();
  check_task_arch_01();
  /* XXX continue tests */
}
