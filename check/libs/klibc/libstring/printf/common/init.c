/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libstring/printf/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [thu dec 29 17:45:46 2005]
 */

#include <klibc.h>
#include "init.h"

void	check_printf_init(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
  printf("init done.\n");
}

/*
 * stars ALL printf tests
 */

void	check_printf_tests(void)
{
  check_printf_init();
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
