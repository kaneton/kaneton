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

static void	check_timer_handler(void)
{
  printf("Timer shot\n");
  timed++;
}

/*
 * XXX
 */

void		check_time_repeat_01(void)
{
  i_timer	id;
  t_uint32	start;
  t_uint32	toto = 0;

  TEST_ENTER();

  ASSERT(timer_reserve(TIMER_FUNCTION, TIMER_HANDLER(check_timer_handler), 0,
		       300, TIMER_REPEAT_ENABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    {
      if (timed == 1 && !toto)
	{
	  timer_repeat(id, TIMER_REPEAT_DISABLE);
	  toto = 1;
	}
    }

  ASSERT(timed == 2, "Timer failed\n");

  TEST_LEAVE();
}
