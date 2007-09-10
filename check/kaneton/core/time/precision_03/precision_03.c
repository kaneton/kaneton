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

void		check_time_precision_03(void)
{
  i_timer	id;
  t_uint32	start;

  TEST_ENTER();

  printf("Timer precision 32ms\n");

  ASSERT(timer_reserve(TIMER_FUNCTION, TIMER_HANDLER(check_timer_handler), 0,
		       1000, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");

  check_rtc_init();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && !timed)
    ;

  ASSERT(timed == 1, "Timer failed\n");

  if (ticks < 968 || ticks > 1032)
    printf("Imprecise timer ! ticks = %u\n", ticks);

  TEST_LEAVE();
}
