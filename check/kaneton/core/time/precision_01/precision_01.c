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

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile t_uint32 timed = 0;

static volatile t_uint32 ticks = 0;

static void	check_timer_handler(void)
{
  ticks = check_cmos_sec();
  timed = 1;
}

/*
 * XXX
 */

void		check_time_precision_01(void)
{
  i_timer	id;
  t_uint32	start;

  TEST_ENTER();

  printf("Timer precision 1000ms\n");

  start = check_cmos_sec();
  while (start == check_cmos_sec())
    ;

  ASSERT(timer_reserve(EVENT_FUNCTION, TIMER_HANDLER(check_timer_handler),
		       500, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  ASSERT(timed == 1, "Timer failed\n");

  if (ticks - start > 1)
    printf("Imprecise timer ! ticks = %u\n", (ticks - start) * 1000);

  TEST_LEAVE();
}
