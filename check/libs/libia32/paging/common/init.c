/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/paging/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [mon jan  9 11:25:01 2006]
 */

#include <klibc.h>
#include "init.h"

void	check_paging_init(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
  printf("init done.\n");
}

/*
 * start ALL paging tests
 */

void	check_paging_tests(void)
{
  check_paging_init();
  check_paging_01();
  /* XXX */
}
