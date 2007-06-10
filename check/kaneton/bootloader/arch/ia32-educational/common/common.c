/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/bootloader/arch/ia32-virtual/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [sat oct 14 16:45:02 2006]
 */

#include <libc.h>
#include <kaneton.h>
#include "common.h"

extern t_init*	init;

/*
 * empty test.
 */

void		check_ia32_virtual_common(void)
{
  /*
   * this test is empty since the "common" test is used to inject code
   * if needed.
   */
}

/*
 * run all bootloader tests.
 */

void		check_bootloader_tests(void)
{
  check_ia32_virtual_common();
  check_ia32_virtual_01();
  check_ia32_virtual_02();
  check_ia32_virtual_03();
  check_ia32_virtual_04();
  /* XXX */
}
