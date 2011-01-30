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
 * updated       julien quintard   [sun jan 30 21:12:33 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

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

void			test_core_timer_repeat_01_content(void)
{
  s_clock		clock;
  t_uint64		start;
  i_timer		tid;
  i_cpu			cpu;

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_repeat_01_handler),
		    TIMER_DATA(NULL),
		    1500,
		    TIMER_OPTION_REPEAT,
		    &tid) != ERROR_OK)
    TEST_HANG("[timer_reserve] error");

  if (clock_current(&clock) != ERROR_OK)
    TEST_HANG("[clock_current] error");

  start = CLOCK_UNIQUE(&clock);

  while (1)
    {
      t_uint64		current;

      if (clock_current(&clock) != ERROR_OK)
	TEST_HANG("[clock_current] error");

      current = CLOCK_UNIQUE(&clock);

      if (current > (start + 3500))
	break;
    }

  if (timed != 2)
    TEST_HANG("the timer has not been triggered twice as expected but %u",
	       timed);

  TEST_SIGNATURE(90sf09isck3t32);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_timer_repeat_01(void)
{
  i_thread		thread;
  i_cpu			cpu;

  TEST_ENTER();

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_timer_repeat_01_content,
		     &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(sdiocvjigi4h4h3);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
