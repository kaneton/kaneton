/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/core/thread/wait/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon feb  7 21:23:42 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile i_thread	thread1;
static volatile i_thread	thread2;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_thread_wait_05_thread_01(void)
{
  s_wait		wait;
  i_cpu			cpu;

  if (thread_wait(thread1, thread2, WAIT_STATE_STOP, &wait) != STATUS_OK)
    TEST_HANG("[thread_wait] error");

  if (WAIT_THREAD(&wait) != thread2)
    TEST_HANG("invalid waiting thread identifier");

  if (WAIT_CAUSE(&wait) != WAIT_STATE_STOP)
    TEST_HANG("invalid waiting cause");

  TEST_SIGNATURE(9032riw0awrgjog);

  if (thread_sleep(thread1, 3000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_thread_wait_05_thread_02(void)
{
  TEST_SIGNATURE(r29q0rawifgerg0);

  if (thread_sleep(thread2, 1000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (thread_stop(thread2) != STATUS_OK)
    TEST_HANG("[thread_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_thread_wait_05(void)
{
  i_cpu			cpu;

  TEST_ENTER();

  /*
   * thread 1
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_thread_wait_05_thread_01,
		     (i_thread*)&thread1) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread1) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_thread_wait_05_thread_02,
		     (i_thread*)&thread2) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread2) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * scheduler
   */


  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != STATUS_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(23t3409gih09i);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
