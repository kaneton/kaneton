/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/id/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:17:32 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "init.h"

void	check_id_init(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
  printf("init done.\n");
}

/*
 * starts ALL tests related to id manager
 */

void	check_id_tests(void)
{
  check_id_init();
  check_id_01();
  check_id_02();
  check_id_03();
  /* XXX continue tests */
}
