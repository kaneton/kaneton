/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/core/timer/reserve/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [sat feb  5 20:47:57 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		timed = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_timer_reserve_01_handler(t_id		id,
							   t_vaddr	data)
{
  timed++;
}

void			test_core_timer_reserve_01(void)
{
  s_clock		clock;
  t_uint32		start;
  i_timer		tid;
  i_cpu			cpu;

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_reserve_01_handler),
		    TIMER_DATA(NULL),
		    1000,
		    TIMER_OPTION_NONE,
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

      if (current > (start + 3000))
	break;
    }

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  if (timed != 1)
    TEST_ERROR("the timers have not been triggered");

  TEST_SIGNATURE(za90fiowigowehgrh);

  TEST_LEAVE();
}
