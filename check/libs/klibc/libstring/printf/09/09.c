/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/09/09.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:53 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:18 2007]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * invalid format
 */

void	check_printf_09(void)
{
  TEST_ENTER();

  CHECK_PRINTF(printf("%a %d\n", 12), 6);

  TEST_LEAVE();
}
