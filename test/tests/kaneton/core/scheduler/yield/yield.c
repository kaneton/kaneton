/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/scheduler/yield/yield.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri dec  3 22:08:08 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "yield.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;
extern t_init*			_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile i_thread	thread_01;
static volatile i_thread	thread_02;

static volatile t_uint32	executed_01 = 0;
static volatile t_uint32	executed_02 = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_scheduler_yield_thread_01(void)
{
  executed_01++;

  if (scheduler_yield() != ERROR_OK)
    TEST_HANG("[scheduler_yield] error");

  executed_01++;

  while (1)
    ;
}

void			test_core_scheduler_yield_thread_02(void)
{
  while (executed_01 == 0)
    ;

  executed_02++;

  if (thread_stop(thread_01) != ERROR_OK)
    TEST_HANG("[thread_stop] error");

  if (thread_stop(thread_02) != ERROR_OK)
    TEST_HANG("[thread_stop] error");

  executed_02++;

  while (1)
    ;
}

void			test_core_scheduler_yield_content(void)
{
  t_thread_context	ctx;
  t_stack		stack;
  o_thread*		t;
  t_clock		clock;
  t_uint64		start;

  /*
   * thread 1
   */

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_01) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_01, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread_01, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_yield_thread_01;

  if (thread_load(thread_01, ctx) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread_01) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_02) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_02, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread_02, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_yield_thread_02;

  if (thread_load(thread_02, ctx) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread_02) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * sleep
   */

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

  /*
   * check
   */

  if ((executed_01 != 1) ||
      (executed_02 != 1))
    TEST_HANG("the threads have not been scheduled/stopped properly or the "
	      "execution has not be yielded as expected");

  /*
   * scheduler
   */

  TEST_SIGNATURE(f923ufwjvc90fg29g);

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_scheduler_yield(void)
{
  i_thread		thread;
  t_thread_context	ctx;
  t_stack		stack;
  o_thread*		t;

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_yield_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(ncvwq2jfd0f309k30bbb);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
