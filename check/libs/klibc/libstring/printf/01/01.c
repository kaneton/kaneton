/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libstring/printf/01/01.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:07 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:25:45 2005]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * simple printf with no substitutions.
 */
void check_printf_01(void)
{
  TEST_ENTER;

  CHECK_PRINTF(printf("toto\n"), 5);
  CHECK_PRINTF(printf("tata\n"), 5);
  CHECK_PRINTF(printf("paul\n"), 5);

  TEST_LEAVE;
}
