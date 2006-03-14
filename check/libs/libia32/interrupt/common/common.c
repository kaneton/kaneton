/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/libs/libia32/interrupt/common/common.c
 *
 * created       renaud voltz   [mon mar 13 12:33:58 2006]
 * updated       renaud voltz   [mon mar 13 12:33:58 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_interrupt_common(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
}

/*
 * start ALL interrupt tests
 */

void	check_interrupt_tests(void)
{
  check_interrupt_common();
  check_interrupt_01();
  check_interrupt_02();
  /* XXX */
}
