/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/05/05.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:53 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:35 2007]
 */

#include <libc/libc.h>
#include "../common/common.h"

/*
 * integer substitution
 */

void	check_printf_05(void)
{
  TEST_ENTER();

  CHECK_PRINTF(printf("%d %d %i %u\n", 0, 12, 24, 42), 11);

  CHECK_PRINTF(printf("%ld %qU\n", -123456L, 123456789123456789LL), 27);

  printf("%d %d %u %u\n", SINT_MIN, SINT_MAX, UINT_MIN, UINT_MAX);

  printf("%ld %ld %lu %lu\n", SLONG_MIN, SLONG_MAX, ULONG_MIN, ULONG_MAX);

  printf("%qd %qd %qu %qu\n", SQUAD_MIN, SQUAD_MAX, UQUAD_MIN, UQUAD_MAX);

  CHECK_PRINTF(printf("%5d %.5d %8.5d %8.5d\n", 42, 42, 42, 424242), 30);

  TEST_LEAVE();
}
