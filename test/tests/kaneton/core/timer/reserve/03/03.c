/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/core/timer/reserve/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat feb  5 20:50:21 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		timed = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_timer_reserve_03_handler_01(void)
{
  timed |= (1 << 0);
}

void			test_core_timer_reserve_03_handler_02(void)
{
  timed |= (1 << 1);
}

void			test_core_timer_reserve_03_handler_03(void)
{
  timed |= (1 << 2);
}

void			test_core_timer_reserve_03_handler_04(void)
{
  timed |= (1 << 3);
}

void			test_core_timer_reserve_03(void)
{
  i_timer		tid;
  s_clock		clock;
  t_uint64		start;

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_reserve_03_handler_01),
		    TIMER_DATA(NULL),
		    1000,
		    TIMER_OPTION_NONE,
		    &tid) != STATUS_OK)
    TEST_ERROR("[timer_reserve] error");

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_reserve_03_handler_02),
		    TIMER_DATA(NULL),
		    200,
		    TIMER_OPTION_NONE,
		    &tid) != STATUS_OK)
    TEST_ERROR("[timer_reserve] error");

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_reserve_03_handler_03),
		    TIMER_DATA(NULL),
		    500,
		    TIMER_OPTION_NONE,
		    &tid) != STATUS_OK)
    TEST_ERROR("[timer_reserve] error");

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_reserve_03_handler_04),
		    TIMER_DATA(NULL),
		    700,
		    TIMER_OPTION_NONE,
		    &tid) != STATUS_OK)
    TEST_ERROR("[timer_reserve] error");

  if (clock_current(&clock) != STATUS_OK)
    TEST_ERROR("[clock_current] error");

  start = CLOCK_UNIQUE(&clock);

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

  while (1)
    {
      t_uint64		current;

      if (clock_current(&clock) != STATUS_OK)
	TEST_ERROR("[clock_current] error");

      current = CLOCK_UNIQUE(&clock);

      if (current > (start + 3000))
	break;
    }

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  if (timed != 0xf)
    TEST_ERROR("the timers have not been properly triggered");

  TEST_SIGNATURE(vmqop3jfwsdv093);

  TEST_LEAVE();
}
