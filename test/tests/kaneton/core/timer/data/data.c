/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/core/timer/data/data.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat feb  5 20:31:29 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "data.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		timed = 0;
static volatile i_timer		tid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_timer_data_handler(t_id	id,
						     t_vaddr	data)
{
  timed = 1;

  if (tid != id)
    TEST_ERROR("invalid timer's identifier");

  if (data != 0x41414141)
    TEST_ERROR("invalid timer's data");
}
 
void			test_core_timer_data(void)
{
  s_clock		clock;
  t_uint64		start;

  TEST_ENTER();

  if (timer_reserve(TIMER_TYPE_FUNCTION,
                    TIMER_ROUTINE(test_core_timer_data_handler),
                    TIMER_DATA(0x41414141),
                    1000,
                    TIMER_OPTION_NONE,
                    (i_timer*)&tid) != STATUS_OK)
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

  TEST_SIGNATURE(weojrgfq290349);

  TEST_LEAVE();
}
