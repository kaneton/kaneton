/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/01/01.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:07 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:53 2007]
 */

#include <libc.h>
#include "../common/common.h"

/*
 * simple printf with no substitutions.
 */
void check_printf_01(void)
{
  TEST_ENTER();

  CHECK_PRINTF(printf("toto\n"), 5);
  CHECK_PRINTF(printf("tata\n"), 5);
  CHECK_PRINTF(printf("paul\n"), 5);

  TEST_LEAVE();
}
