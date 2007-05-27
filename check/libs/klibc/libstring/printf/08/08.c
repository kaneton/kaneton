/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/08/08.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:53 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:22 2007]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * hex and octal and bin
 */

void	check_printf_08(void)
{
  char	*p = (char*)0xffff4f05;

  TEST_ENTER();

  CHECK_PRINTF(printf("%x %qX %p\n", 0xabcdef98, 0xfedcba9876543210LL, p), 35);

  CHECK_PRINTF(printf("0x%x 0x%x\n", 0xffffffff, 0x0), 15);

  CHECK_PRINTF(printf("%o %O %o\n", 00, 0777, 01234), 11);

  CHECK_PRINTF(printf("%b %qb\n", 42, 0xffffffff11LL), 48);

  TEST_LEAVE();
}
