/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:43:46 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_libia32_common(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
}

/*
 * start ALL ia32 tests
 */

void	check_libia32_tests(void)
{
  check_libia32_common();
  /*  check_pmode_tests();
      check_paging_tests();*/
  check_interrupt_tests();
  check_time_tests();
  /* XXX */
}

/*
 * #gp exception handler
 */

void	fault_handler(void)
{
  printf("SYSTEM THROWN GENERAL PROTECTION FAULT\n");
  while (1);
}

/*
 * initialise the exception handler
 */

void	init_fault_handler(void)
{
  // XXX
}

/*
 * resets the exception handler
 */

void	clean_fault_handler(void)
{
  // XXX
}
