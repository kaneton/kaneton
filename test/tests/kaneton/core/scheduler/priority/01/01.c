/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../kaneton/core/scheduler/priority/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:18:51 2011]
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

static volatile t_uint32	executed_01 = 0;
static volatile t_uint32	executed_02 = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_scheduler_priority_01_thread_01(void)
{
  while (1)
    {
      executed_01++;
    }
}

void			test_core_scheduler_priority_01_thread_02(void)
{
  while (1)
    {
      executed_02++;
    }
}

void			test_core_scheduler_priority_01_content(void)
{
  i_thread		thread;
  s_clock		clock;
  t_uint64		start;
  i_cpu			cpu;

  /*
   * thread 1
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY_HIGH,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_01_thread_01,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_01_thread_02,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
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

  if ((executed_01 == 0) || (executed_02 == 0))
    TEST_HANG("one or both of the threads has/have not been executed");

  if (executed_01 <= executed_02)
    TEST_HANG("the thread with the highest priority has not been executed "
	      "more times than the other: %u against %u\n",
	      executed_01, executed_02);

  /*
   * stop
   */

  TEST_SIGNATURE(nsdv09wi23f0w9gv);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_scheduler_priority_01(void)
{
  i_thread		thread;
  i_cpu			cpu;

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_01_content,
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

  TEST_SIGNATURE(mvnweifjq2309gi34g);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
