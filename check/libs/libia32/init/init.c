/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [mon jan  9 12:38:47 2006]
 */

#include <klibc.h>
#include "init.h"

void	check_libia32_init(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
  printf("init done.\n");
}

/*
 * start ALL ia32 tests
 */

void	check_libia32_tests(void)
{
  check_pmode_tests();
  check_paging_tests();
  /* XXX */
}

/*
 * #gp exception handler
 */

void	fault_handler()
{
  printf("SYSTEM THROWN GENERAL PROTECTION FAULT\n");
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
