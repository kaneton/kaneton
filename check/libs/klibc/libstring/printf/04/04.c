/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/04/04.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:53 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:40 2007]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * empty strings
 */

void	check_printf_04(void)
{
  TEST_ENTER();

  CHECK_PRINTF(printf(""), 0);

  CHECK_PRINTF(printf("%s%s%s%s", "", "", "", ""), 0);

  TEST_LEAVE();
}
