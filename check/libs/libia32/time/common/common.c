/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/libs/libia32/time/common/common.c
 *
 * created       renaud voltz   [mon mar 13 12:33:58 2006]
 * updated       renaud voltz   [mon mar 13 12:33:58 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_time_common(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
}

/*
 * start ALL time tests
 */

void	check_time_tests(void)
{
  check_time_common();
  check_time_01();
  /* XXX */
}
