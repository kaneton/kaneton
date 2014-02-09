/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/timer/precision/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:01 2010]
 * updated       julien quintard   [sat feb  5 20:34:51 2011]
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

static volatile t_uint64	begin = 0;
static volatile t_uint64	end = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_timer_precision_01_handler(t_id	id,
							     t_vaddr	data)
{
  s_clock		clock;

  timed = 1;

  if (clock_current(&clock) != STATUS_OK)
    TEST_ERROR("[clock_current] error");

  end = CLOCK_UNIQUE(&clock);
}

void			test_core_timer_precision_01(void)
{
  s_clock		clock;
  t_uint64		start;
  i_timer		tid;

  TEST_ENTER();

  if (clock_current(&clock) != STATUS_OK)
    TEST_ERROR("[clock_current] error");

  begin = CLOCK_UNIQUE(&clock);

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_precision_01_handler),
		    TIMER_DATA(NULL),
		    500,
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

  if (timed != 1)
    TEST_ERROR("the timer has not been triggered");

  if ((end - begin) > 1000)
    TEST_ERROR("imprecise timer: %u milliseconds", end - begin);

  TEST_SIGNATURE(sdff3f0jg4g0934);

  TEST_LEAVE();
}
