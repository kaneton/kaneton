/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/id/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [wed jan 18 18:53:18 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_id_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to id manager
 */

void	check_id_tests(void)
{
  check_id_common();
  check_id_01();
  check_id_02();
  check_id_03();
  /* XXX continue tests */
}
