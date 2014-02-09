/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/timer/repeat/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:01 2010]
 * updated       julien quintard   [sat feb  5 20:40:29 2011]
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
 
void			test_core_timer_repeat_01_handler(t_id		id,
							  t_vaddr	data)
{
  timed++;
}

void			test_core_timer_repeat_01(void)
{
  s_clock		clock;
  t_uint64		start;
  i_timer		tid;

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_repeat_01_handler),
		    TIMER_DATA(NULL),
		    1500,
		    TIMER_OPTION_REPEAT,
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

      if (current > (start + 3500))
	break;
    }

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  if (timed != 2)
    TEST_ERROR("the timer has not been triggered twice as expected but %u",
	       timed);

  TEST_SIGNATURE(sdiocvjigi4h4h3);

  TEST_LEAVE();
}
