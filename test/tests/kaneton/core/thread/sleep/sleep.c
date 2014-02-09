/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/thread/sleep/sleep.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:09:55 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "sleep.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile i_thread	thread;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_thread_sleep_content(void)
{
  s_clock		clock;
  t_uint64		past;
  t_uint64		current;
  i_cpu			cpu;

  if (clock_current(&clock) != STATUS_OK)
    TEST_HANG("[clock_current] error");

  past = CLOCK_UNIQUE(&clock);

  TEST_SIGNATURE(34fwop843otj);

  if (thread_sleep(thread, 3000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (clock_current(&clock) != STATUS_OK)
    TEST_HANG("[clock_current] error");

  current = CLOCK_UNIQUE(&clock);

  if (((current - past) < 2000) ||
      ((current - past) > 4000))
    TEST_HANG("the thread has been put to sleep for an invalid number of "
	       "milliseconds");

  TEST_SIGNATURE(12sakpow3208);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_thread_sleep(void)
{
  i_cpu			cpu;

  TEST_ENTER();

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
		     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_thread_sleep_content,
		     (i_thread*)&thread) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != STATUS_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(adrf3r3kfwef0);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
