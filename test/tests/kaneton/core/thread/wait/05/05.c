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
 * updated       julien quintard   [fri dec  3 16:12:21 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile i_thread	thread1;
static volatile i_thread	thread2;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_thread_wait_05_content_01(void)
{
  t_wait		wait;

  if (thread_wait(WAIT_STATE_STOP, thread2, &wait) != ERROR_OK)
    TEST_HANG("[thread_wait] error");

  if (WAIT_THREAD(&wait) != thread2)
    TEST_HANG("invalid waiting thread identifier");

  if (WAIT_CAUSE(&wait) != WAIT_STATE_STOP)
    TEST_HANG("invalid waiting cause");

  TEST_SIGNATURE(9032riw0awrgjog);

  if (thread_sleep(3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_thread_wait_05_content_02(void)
{
  TEST_SIGNATURE(r29q0rawifgerg0);

  if (thread_sleep(1000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (thread_stop(thread2) != ERROR_OK)
    TEST_HANG("[thread_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_thread_wait_05(void)
{
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;

  TEST_ENTER();

  /*
   * thread 1
   */

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     (i_thread*)&thread1) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread1, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread1, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_thread_wait_05_content_01;

  if (thread_load(thread1, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread1) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     (i_thread*)&thread2) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread2, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread2, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_thread_wait_05_content_02;

  if (thread_load(thread2, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread2) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * scheduler
   */

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(23t3409gih09i);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
