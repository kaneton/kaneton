/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/paging/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:49:43 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_paging_common(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
}

/*
 * start ALL paging tests
 */

void	check_paging_tests(void)
{
  check_paging_common();
  check_paging_01();
  /* XXX */
}
