/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/07/07.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:53 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:27 2007]
 */

#include <libc.h>
#include "../common/common.h"

/*
 * chars
 */

void	check_printf_07(void)
{
  TEST_ENTER();

  CHECK_PRINTF(printf("A%c%c%c%c%c%c un char et un %cutr%c\n", 'f', 'f', 'i', 'c', 'h', 'e', 'a', 'e'), 28);

  TEST_LEAVE();
}
