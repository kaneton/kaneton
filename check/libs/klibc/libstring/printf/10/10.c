/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/10/10.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:53 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:14 2007]
 */

#include <libc/libc.h>
#include "../common/common.h"

/*
 * complex test
 */

void	check_printf_10(void)
{
  TEST_ENTER();

  printf("Un petit combo : %%%d%u%s%c%x%o\n%%%d%u%s%c%x%o\n%%%d%u%s%c%x%o\n%%%d%u%s%c%x%o\n%%%d%u%s%c%x%o\n",
	 -42, 42, "42", 42, 42, 042,
	 -42, 42, "42", 42, 42, 042,
	 -42, 42, "42", 42, 42, 042,
	 -42, 42, "42", 42, 42, 042,
	 -42, 42, "42", 42, 42, 042);

  TEST_LEAVE();
}
