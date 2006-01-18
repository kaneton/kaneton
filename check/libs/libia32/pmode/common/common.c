/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/pmode/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:45:41 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_pmode_common(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
}

/*
 * start ALL pmode tests
 */

void	check_pmode_tests(void)
{
  check_pmode_common();
  check_pmode_01();
  check_pmode_02();
  check_pmode_03();
  /* XXX */
}
