/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [fri mar 17 13:50:56 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_segment_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to segment manager
 */

void	check_segment_tests(void)
{
  check_segment_common();
  check_segment_01();
  check_segment_02();
  check_segment_03();
  check_segment_04();
  check_segment_05();
  check_segment_06();
  check_segment_07();
  check_segment_08();
  check_segment_09();
  /* XXX continue tests */
}
