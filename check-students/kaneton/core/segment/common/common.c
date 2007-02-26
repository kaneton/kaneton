/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [thu jun  1 18:30:16 2006]
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
  check_segment_simple_reserve_01();
  check_segment_simple_reserve_02();
  check_segment_simple_reserve_03();
  check_segment_multiple_reserve_01();
  check_segment_multiple_reserve_02();
  check_segment_multiple_reserve_03();
}
