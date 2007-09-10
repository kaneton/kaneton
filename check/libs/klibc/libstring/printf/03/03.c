/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/03/03.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:53 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:44 2007]
 */

#include <libc/libc.h>
#include "../common/common.h"

/*
 * escape characters.
 */

void	check_printf_03(void)
{
  TEST_ENTER();

  CHECK_PRINTF(printf("\thorizontal tabulation"), 22);

  CHECK_PRINTF(printf("\n\nnewline\n"), 10);

  CHECK_PRINTF(printf("carrr\rcarriage return "), 22);

  CHECK_PRINTF(printf("backk\bspace"), 11);

  CHECK_PRINTF(printf("%%\n"), 2);

  TEST_LEAVE();
}
