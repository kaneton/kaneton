/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [fri mar 24 16:07:26 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_as_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to as manager
 */

void	check_as_tests(void)
{
  check_as_common();
/*  check_as_01();
  check_as_02();
  check_as_03();
  check_as_04();
  check_as_05();*/
  check_as_06();
//check_as_07();
  /* XXX continue tests */
}
