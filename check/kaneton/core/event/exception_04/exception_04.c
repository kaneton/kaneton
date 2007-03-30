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

static volatile int	div = 0;
static volatile int	thrown = 0;

static void	check_dv(t_id	id)
{
  printf(" + Exception thrown\n");
  thrown = 1;
  div = 1;
}

/*
 * reserve event onto div by zero.
 */

void		check_event_exception_04(void)
{
  TEST_ENTER();

  ASSERT(event_reserve(0,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_dv)) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Throwing exception\n");
  asm volatile("divl %0"
	       :
	       : "m" (div));
  printf(" - Execution resumed\n");

  ASSERT(thrown == 1, " x Exception not caught\n");

  TEST_LEAVE();
}
