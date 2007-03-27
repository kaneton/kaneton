/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int	thrown = 0;

static void	check_int3(t_id	id)
{
  printf(" + Exception thrown\n");
  ASSERT(id == 3, " x Bad event identifier passed to handler\n");
  thrown = 1;
}

/*
 * reserve event onto int3 and throw an int3.
 */

void		check_event_exception_02(void)
{
  TEST_ENTER();

  ASSERT(event_reserve(3,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_int3)) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Throwing exception\n");
  asm volatile("int $3");
  ASSERT(thrown == 1, " x Exception not caught\n");
  printf(" - Execution resumed\n");

  TEST_LEAVE();
}
