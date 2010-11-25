/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ine/glue/ibm-pc.ia32/time/repeat/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov 20 23:31:23 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		timed = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ibmpc_ia32_time_repeat_01_handler(t_id	id,
							       t_vaddr	data)
{
  timed++;
}

void			test_ibmpc_ia32_time_repeat_01_content(void)
{
  t_uint32		start;
  i_timer		tid;
  t_uint32		flag;

  if (timer_reserve(TIMER_FUNCTION,
		    TIMER_HANDLER(test_ibmpc_ia32_time_repeat_01_handler),
		    0,
		    300,
		    TIMER_REPEAT_ENABLE,
		    &tid) != ERROR_OK)
    TEST_ERROR("[event_reserve] error\n");

  flag = 0;

  start = test_ibmpc_ia32_time_cmos();
  while (((start + 3) % 60) != test_ibmpc_ia32_time_cmos())
    {
      if (timed == 1 && flag == 0)
	{
	  timer_repeat(tid, TIMER_REPEAT_DISABLE);
	  flag = 1;
	}
    }

  if (timed != 2)
    TEST_ERROR("the timer has not been triggered twice as expected\n");

  while (1)
    ;
}

void			test_ibmpc_ia32_time_repeat_01(void)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  TEST_ENTER();

  if (thread_reserve(_kernel->task, THREAD_PRIOR, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_ibmpc_ia32_time_repeat_01_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_OK)
    TEST_ERROR("[ia32_get_context] error\n");

  if (scheduler_add(thread) != ERROR_OK)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  TEST_LEAVE();
}
