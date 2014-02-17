/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../kaneton/core/scheduler/priority/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:18:58 2011]
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

static volatile t_uint32	executed[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_scheduler_priority_02_thread_01(void)
{
  while (1)
    {
      executed[0]++;
    }
}

void			test_core_scheduler_priority_02_thread_02(void)
{
  while (1)
    {
      executed[1]++;
    }
}

void			test_core_scheduler_priority_02_thread_03(void)
{
  while (1)
    {
      executed[2]++;
    }
}

void			test_core_scheduler_priority_02_thread_04(void)
{
  while (1)
    {
      executed[3]++;
    }
}

void			test_core_scheduler_priority_02_thread_05(void)
{
  while (1)
    {
      executed[4]++;
    }
}

void			test_core_scheduler_priority_02_thread_06(void)
{
  while (1)
    {
      executed[5]++;
    }
}

void			test_core_scheduler_priority_02_thread_07(void)
{
  while (1)
    {
      executed[6]++;
    }
}

void			test_core_scheduler_priority_02_thread_08(void)
{
  while (1)
    {
      executed[7]++;
    }
}

void			test_core_scheduler_priority_02_content(void)
{
  i_thread		thread;
  t_uint32		i;
  s_clock		clock;
  t_uint64		start;
  i_cpu			cpu;

  /*
   * thread 1
   */

  if (thread_reserve(_kernel.task,
		     10,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_thread_01,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel.task,
		     40,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_thread_02,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 3
   */

  if (thread_reserve(_kernel.task,
		     70,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_thread_03,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 4
   */

  if (thread_reserve(_kernel.task,
		     100,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_thread_04,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 5
   */

  if (thread_reserve(_kernel.task,
		     130,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_thread_05,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 6
   */

  if (thread_reserve(_kernel.task,
		     160,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_thread_06,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 7
   */

  if (thread_reserve(_kernel.task,
		     190,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_thread_07,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 8
   */

  if (thread_reserve(_kernel.task,
		     220,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_thread_08,
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

      if (current > (start + 15000))
	break;
    }

  /*
   * check
   */

  for (i = 0; i < 8; i++)
    {
      if (executed[i] == 0)
	TEST_HANG("some of the threads have not been executed");
    }

  for (i = 0; i < 7; i++)
    {
      if (executed[i] >= executed[i + 1])
	TEST_HANG("one of the thread with a higher priority has not been "
		  "executed more times than an other with a lower priority: "
		  "%u against %u\n",
		  executed[i], executed[i + 1]);
    }

  /*
   * stop
   */

  TEST_SIGNATURE(09fiwfmcv0i3f2waf09);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_scheduler_priority_02(void)
{
  i_thread		thread;
  i_cpu			cpu;

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_priority_02_content,
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

  TEST_SIGNATURE(mccc2q3d09wg93h34);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
