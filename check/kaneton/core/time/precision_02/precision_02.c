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

static volatile t_uint32 timed = 0;

static volatile t_uint32 ticks = 0;

static void	check_timer_handler(void)
{
  ticks = check_rtc_ticks();
  timed = 1;
}

/*
 * XXX
 */

void		check_time_precision_02(void)
{
  i_timer	id;
  t_uint32	start;

  TEST_ENTER();

  printf("Timer precision 200ms\n");

  ASSERT(timer_reserve(EVENT_FUNCTION, TIMER_HANDLER(check_timer_handler),
		       1000, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");

  check_rtc_init();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  ASSERT(timed == 1, "Timer failed\n");

  if (ticks < 800 || ticks > 1200)
    printf("Imprecise timer ! ticks = %u\n", ticks);

  TEST_LEAVE();
}