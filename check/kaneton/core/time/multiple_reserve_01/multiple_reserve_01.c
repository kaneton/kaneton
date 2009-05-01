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

static void	check_timer1_handler(void)
{
  printf("Timer 1 shot\n");
  timed |= (1 << 0);
}

static void	check_timer2_handler(void)
{
  printf("Timer 2 shot\n");
  timed |= (1 << 1);
}

static void	check_timer3_handler(void)
{
  printf("Timer 3 shot\n");
  timed |= (1 << 2);
}

static void	check_timer4_handler(void)
{
  printf("Timer 4 shot\n");
  timed |= (1 << 3);
}

/*
 * XXX
 */

void		check_time_multiple_reserve_01_entry(void)
{
  i_timer	id;
  t_uint32	start;

  TEST_ENTER();

  assert(timer_reserve(TIMER_FUNCTION, TIMER_HANDLER(check_timer1_handler), 0,
		       1000, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");
  assert(timer_reserve(TIMER_FUNCTION, TIMER_HANDLER(check_timer2_handler), 0,
		       200, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");
  assert(timer_reserve(TIMER_FUNCTION, TIMER_HANDLER(check_timer3_handler), 0,
		       500, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");
  assert(timer_reserve(TIMER_FUNCTION, TIMER_HANDLER(check_timer4_handler), 0,
		       700, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  assert(timed == 0xf, "Timer failed\n");

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_time_multiple_reserve_01)
