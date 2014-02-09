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
 * updated       julien quintard   [sun jan 30 21:19:07 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "yield.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

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

  if (scheduler_yield() != STATUS_OK)
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

  if (thread_stop(thread_01) != STATUS_OK)
    TEST_HANG("[thread_stop] error");

  if (thread_stop(thread_02) != STATUS_OK)
    TEST_HANG("[thread_stop] error");

  executed_02++;

  while (1)
    ;
}

void			test_core_scheduler_yield_content(void)
{
  s_clock		clock;
  t_uint64		start;
  i_cpu			cpu;

  /*
   * thread 1
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_yield_thread_01,
		     (i_thread*)&thread_01) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread_01) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_yield_thread_02,
		     (i_thread*)&thread_02) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread_02) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * sleep
   */

  if (clock_current(&clock) != STATUS_OK)
    TEST_HANG("[clock_current] error");

  start = CLOCK_UNIQUE(&clock);

  while (1)
    {
      t_uint64		current;

      if (clock_current(&clock) != STATUS_OK)
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
	      "execution has not be yielded as expected: [%u, %u]",
	      executed_01, executed_02);

  /*
   * scheduler
   */

  TEST_SIGNATURE(f923ufwjvc90fg29g);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_scheduler_yield(void)
{
  i_thread		thread;
  i_cpu			cpu;

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_yield_content,
		     &thread) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != STATUS_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(ncvwq2jfd0f309k30bbb);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
