/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libstring/printf/02/02.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:03:57 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:25:42 2005]
 */

#include <klibc.h>
#include "../init/init.h"

/*
 * simple integer substitution.
 */

void	check_printf_02(void)
{
  TEST_ENTER;

  CHECK_PRINTF(printf("salut\n%d\ntoto\n", 10), 14);

  TEST_LEAVE;
}
