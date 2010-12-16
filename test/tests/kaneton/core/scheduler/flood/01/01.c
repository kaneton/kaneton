/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/scheduler/flood/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu dec 16 12:04:10 2010]
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

static volatile t_uint32	executed = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_scheduler_flood_01_thread(void)
{
  if (event_disable() != ERROR_OK)
    TEST_HANG("[event_disable] error");

  executed++;

  if (event_enable() != ERROR_OK)
    TEST_HANG("[event_enable] error");

  while (1)
    {
      if (scheduler_yield() != ERROR_OK)
	TEST_HANG("[scheduler_yield] error");
    }
}

void			test_core_scheduler_flood_01_content(void)
{
  i_thread		thread;
  s_thread_context	ctx;
  s_stack		stack;
  o_thread*		t;
  t_uint32		i;
  s_clock		clock;
  t_uint64		start;
  i_cpu			cpu;

  /*
   * threads
   */

  for (i = 0; i < 25; i++)
    {
      if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
	TEST_HANG("[thread_reserve] error");

      stack.base = 0;
      stack.size = THREAD_STACKSZ_LOW;

      if (thread_stack(thread, stack) != ERROR_OK)
	TEST_HANG("[thread_stack] error");

      if (thread_get(thread, &t) != ERROR_OK)
	TEST_HANG("[thread_get] error");

      ctx.sp = t->stack + t->stacksz - 16;
      ctx.pc = (t_vaddr)test_core_scheduler_flood_01_thread;

      if (thread_load(thread, ctx) != ERROR_OK)
	TEST_HANG("[thread_load] error");

      if (thread_start(thread) != ERROR_OK)
	TEST_HANG("[thread_start] error");
    }

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

  if (executed != 25)
    TEST_HANG("some of the threads have not been executed");

  /*
   * stop
   */

  TEST_SIGNATURE(r29mnsdv2w3030h);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_scheduler_flood_01(void)
{
  i_thread		thread;
  s_thread_context	ctx;
  s_stack		stack;
  o_thread*		t;
  i_cpu			cpu;

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_flood_01_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(09fw23r2nmwivcw3g93);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
