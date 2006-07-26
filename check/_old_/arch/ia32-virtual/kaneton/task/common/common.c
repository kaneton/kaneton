/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/task/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [sat apr 15 23:01:30 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_task_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to task manager
 */

void	check_task_tests(void)
{
  check_task_common();
  check_task_01();
  /* XXX continue tests */
}