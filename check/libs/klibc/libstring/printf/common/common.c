/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libstring/printf/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:19:29 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_printf_common(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
}

/*
 * stars ALL printf tests
 */

void	check_printf_tests(void)
{
  check_printf_common();
  check_printf_01();
  check_printf_02();
  check_printf_03();
  check_printf_04();
  check_printf_05();
  check_printf_06();
  check_printf_07();
  check_printf_08();
  check_printf_09();
  check_printf_10();
  /* XXX */
}
