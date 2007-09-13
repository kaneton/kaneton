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

static void	check_softint1(t_id	id)
{
  printf(" + Softint1 thrown\n");
  if (id != 56)
    printf(" x Bad event identifier passed to handler\n");
  thrown++;
}

static void	check_softint2(t_id	id)
{
  printf(" + Softint2 thrown\n");
  if (id != 57)
    printf(" x Bad event identifier passed to handler\n");
  thrown++;
}

static void	check_softint3(t_id	id)
{
  printf(" + Softint3 thrown\n");
  if (id != 58)
    printf(" x Bad event identifier passed to handler\n");
  thrown++;
}

/*
 * reserve event onto int 0x42 and call int 0x42.
 */

void		check_event_softint_02(void)
{
  TEST_ENTER();

  event_release(56);
  event_release(57);
  event_release(58);

  ASSERT(event_reserve(56,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_softint1), 0) == ERROR_NONE,
	 "cannot event_reserve\n");
  ASSERT(event_reserve(57,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_softint2), 0) == ERROR_NONE,
	 "cannot event_reserve\n");
  ASSERT(event_reserve(58,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_softint3), 0) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Softint\n");
  asm volatile("int $56");
  asm volatile("int $57");
  asm volatile("int $58");
  ASSERT(thrown == 3, " x One or more softint were not caught\n");
  printf(" - Execution resumed\n");

  TEST_LEAVE();
}
