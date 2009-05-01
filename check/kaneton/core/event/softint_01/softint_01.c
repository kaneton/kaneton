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

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int	thrown = 0;

static void	check_softint(t_id	id)
{
  printf(" + Softint thrown\n");
  if (id != 56)
    printf(" x Bad event identifier passed to handler\n");
  thrown = 1;
}

/*
 * reserve event onto int 56 and call int 56.
 */

void		check_event_softint_01(void)
{
  TEST_ENTER();

  event_release(56);
  assert(event_reserve(56,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_softint), 0) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Softint\n");
  asm volatile("int $56");
  assert(thrown == 1, " x Softint not caught\n");
  printf(" - Execution resumed\n");

  TEST_LEAVE();
}
