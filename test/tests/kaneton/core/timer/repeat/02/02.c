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
 * updated       julien quintard   [sun jan 30 21:12:39 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

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
 
void			test_core_timer_repeat_02_handler(t_id		id,
							  t_vaddr	data)
{
  timed++;
}

void			test_core_timer_repeat_02_content(void)
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

  if (timed != 3)
    TEST_HANG("the timer has not been triggered thrice as expected but %u",
	       timed);

  TEST_SIGNATURE(90cfir32f93g894g);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_timer_repeat_02(void)
{
  i_thread		thread;
  i_cpu			cpu;

  TEST_ENTER();

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_timer_repeat_02_content,
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

  TEST_SIGNATURE(zxccnw32r32g);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
