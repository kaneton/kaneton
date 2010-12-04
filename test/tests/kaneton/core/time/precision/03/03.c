/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/core/time/precision/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:01 2010]
 * updated       julien quintard   [sat dec  4 12:20:44 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		timed = 0;

static volatile t_uint64	begin = 0;
static volatile t_uint64	end = 0;

/*
 * ---------- test ------------------------------------------------------------
 */
 
void			test_core_time_precision_03_handler(t_id	id,
							    t_vaddr	data)
{
  t_clock		clock;

  timed = 1;

  if (clock_current(&clock) != ERROR_OK)
    TEST_ERROR("[clock_current] error");

  end = CLOCK_UNIQUE(&clock);
}

void			test_core_time_precision_03_content(void)
{
  t_clock		clock;
  t_uint64		start;
  i_timer		tid;

  if (clock_current(&clock) != ERROR_OK)
    TEST_HANG("[clock_current] error");

  begin = CLOCK_UNIQUE(&clock);

  if (timer_reserve(TIMER_FUNCTION,
		    TIMER_HANDLER(test_core_time_precision_03_handler),
		    0,
		    1000,
		    TIMER_OPTION_NONE,
		    &tid) != ERROR_OK)
    TEST_HANG("[event_reserve] error");

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

  if (((end - begin) < 968) ||
      ((end - begin) > 1032))
    TEST_HANG("imprecise timer: %u milliseconds", end - begin);

  TEST_SIGNATURE(zxckml3f3209gu8);

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_time_precision_03(void)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;

  TEST_ENTER();

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_time_precision_03_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(ti20e9wfc20392);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
