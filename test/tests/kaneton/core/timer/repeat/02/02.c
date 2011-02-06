/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/timer/repeat/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:01 2010]
 * updated       julien quintard   [sat feb  5 20:41:37 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		timed = 0;

/*
 * ---------- test ------------------------------------------------------------
 */
 
void			test_core_timer_repeat_02_handler(t_id		id,
							  t_vaddr	data)
{
  timed++;
}

void			test_core_timer_repeat_02(void)
{
  s_clock		clock;
  t_uint64		start;
  i_timer		tid;
  i_cpu			cpu;

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_repeat_02_handler),
		    TIMER_DATA(NULL),
		    1000,
		    TIMER_OPTION_REPEAT,
		    &tid) != ERROR_OK)
    TEST_ERROR("[timer_reserve] error");

  if (clock_current(&clock) != ERROR_OK)
    TEST_ERROR("[clock_current] error");

  start = CLOCK_UNIQUE(&clock);

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  while (1)
    {
      t_uint64		current;

      if (clock_current(&clock) != ERROR_OK)
	TEST_ERROR("[clock_current] error");

      current = CLOCK_UNIQUE(&clock);

      if (current > (start + 3500))
	break;
    }

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  if (timed != 3)
    TEST_ERROR("the timer has not been triggered thrice as expected but %u",
	       timed);

  TEST_SIGNATURE(zxccnw32r32g);

  TEST_LEAVE();
}
