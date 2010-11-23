/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../glue/ibm-pc.ia32/time/message/message.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov 20 23:21:08 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "message.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task			ktask;

/*
 * ---------- test ------------------------------------------------------------
 */
 
void			test_ibmpc_ia32_time_message_content(void)
{
  t_uint32		start;
  i_timer		tid;
  o_timer_message	evt;
  t_vsize		sz;
  i_node		sender;

  start = test_ibmpc_ia32_time_cmos();
  while (start == test_ibmpc_ia32_time_cmos())
    ;

  if (timer_reserve(TIMER_FUNCTION,
		    TIMER_TASK(ktask),
		    0x41414141,
		    500,
		    TIMER_REPEAT_DISABLE,
		    &tid) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  start = test_ibmpc_ia32_time_cmos();
  while (((start + 3) % 60) != test_ibmpc_ia32_time_cmos())
    ;

  if (message_poll(ktask, MESSAGE_TYPE_TIMER, (t_vaddr)&evt,
		   &sz, &sender) != ERROR_NONE)
    TEST_ERROR("[message_poll] error: the timer has not been triggered\n");

  if (evt.id != tid)
    TEST_ERROR("invalid event's identifier\n");

  if (evt.data != 0x41414141)
    TEST_ERROR("invalid event's data\n");

  if (sender.task != 0)
    TEST_ERROR("invalid sender's task identifier\n");

  while (1)
    ;
}

void			test_ibmpc_ia32_time_message(void)
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
  ctx.pc = (t_vaddr)test_ibmpc_ia32_time_message_content;

  if (thread_load(thread, ctx) != ERROR_NONE)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_NONE)
    TEST_ERROR("[ia32_get_context] error\n");

  if (scheduler_add(thread) != ERROR_NONE)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  TEST_LEAVE();
}
