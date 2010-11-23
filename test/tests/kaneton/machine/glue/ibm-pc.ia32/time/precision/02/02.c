/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../glue/ibm-pc.ia32/time/precision/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:02 2020]
 * updated       julien quintard   [sat nov 20 23:43:03 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task			ktask;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		timed = 0;
static volatile t_uint32	ticks = 0;

/*
 * ---------- test ------------------------------------------------------------
 */
 
void			test_ibmpc_ia32_time_precision_02_handler(t_id	  id,
								  t_vaddr data)
{
  timed = 1;

  ticks = test_ibmpc_ia32_time_rtc();
}

void			test_ibmpc_ia32_time_precision_02_content(void)
{
  t_uint32		start;
  i_timer		tid;

  if (timer_reserve(TIMER_FUNCTION,
		    TIMER_HANDLER(test_ibmpc_ia32_time_precision_02_handler),
		    0,
		    1000,
		    TIMER_REPEAT_DISABLE,
		    &tid) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  test_ibmpc_ia32_time_rtc_setup();

  start = test_ibmpc_ia32_time_cmos();
  while (((start + 3) % 60) != test_ibmpc_ia32_time_cmos() &&
	 !timed)
    ;

  if (timed != 1)
    TEST_ERROR("the timer has not been triggered\n");

  if (ticks < 800 || ticks > 1200)
    TEST_ERROR("imprecise timer: %u ticks\n", ticks);

  while (1)
    ;
}

void			test_ibmpc_ia32_time_precision_02(void)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  TEST_ENTER();

  if (thread_reserve(ktask, THREAD_PRIOR, &thread) != ERROR_NONE)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_NONE)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_NONE)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_ibmpc_ia32_time_precision_02_content;

  if (thread_load(thread, ctx) != ERROR_NONE)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_NONE)
    TEST_ERROR("[ia32_get_context] error\n");

  if (scheduler_add(thread) != ERROR_NONE)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  TEST_LEAVE();
}
