/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/pmode/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [mon jan  9 12:01:54 2006]
 */

#include <klibc.h>
#include "init.h"

void	check_pmode_init(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
  printf("init done.\n");
}

/*
 * start ALL pmode tests
 */

void	check_pmode_tests(void)
{
  check_pmode_init();
  check_pmode_01();
  check_pmode_02();
  check_pmode_03();
  /* XXX */
}
