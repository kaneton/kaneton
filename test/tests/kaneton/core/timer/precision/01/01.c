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
 * updated       julien quintard   [sun jan 30 21:12:12 2011]
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

  if (clock_current(&clock) != ERROR_OK)
    TEST_ERROR("[clock_current] error");

  end = CLOCK_UNIQUE(&clock);
}

void			test_core_timer_precision_01_content(void)
{
  s_clock		clock;
  t_uint64		start;
  i_timer		tid;
  i_cpu			cpu;

  if (clock_current(&clock) != ERROR_OK)
    TEST_HANG("[clock_current] error");

  begin = CLOCK_UNIQUE(&clock);

  if (timer_reserve(TIMER_TYPE_FUNCTION,
		    TIMER_ROUTINE(test_core_timer_precision_01_handler),
		    TIMER_DATA(NULL),
		    500,
		    TIMER_OPTION_NONE,
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

      if (current > (start + 3000))
	break;
    }

  if (timed != 1)
    TEST_HANG("the timer has not been triggered");

  if ((end - begin) > 1000)
    TEST_HANG("imprecise timer: %u milliseconds", end - begin);

  TEST_SIGNATURE(t3490gjsdof29);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_timer_precision_01(void)
{
  i_thread		thread;
  i_cpu			cpu;

  TEST_ENTER();

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_timer_precision_01_content,
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

  TEST_SIGNATURE(sdff3f0jg4g0934);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
