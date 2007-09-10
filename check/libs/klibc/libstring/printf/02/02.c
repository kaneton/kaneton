/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/02/02.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:03:57 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:49 2007]
 */

#include <libc/libc.h>
#include "../common/common.h"

/*
 * simple integer substitution.
 */

void	check_printf_02(void)
{
  TEST_ENTER();

  CHECK_PRINTF(printf("salut\n%d\ntoto\n", 10), 14);

  TEST_LEAVE();
}
