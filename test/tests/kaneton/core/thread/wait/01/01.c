/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/core/thread/wait/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat dec 18 15:45:37 2010]
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

static volatile i_thread	thread1;
static volatile i_thread	thread2;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_thread_wait_01_thread_01(void)
{
  s_wait		wait;
  i_cpu			cpu;

  if (thread_wait(thread1, thread2, WAIT_STATE_DEATH, &wait) != ERROR_OK)
    TEST_HANG("[thread_wait] error");

  if (WAIT_THREAD(&wait) != thread2)
    TEST_HANG("invalid waiting thread identifier");

  if (WAIT_CAUSE(&wait) != WAIT_STATE_DEATH)
    TEST_HANG("invalid waiting cause");

  if (WAIT_VALUE(&wait) != 42)
    TEST_HANG("invalid waiting value");

  TEST_SIGNATURE(w0wekodf3afdwfa0);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_thread_wait_01_thread_02(void)
{
  TEST_SIGNATURE(wee3ri20raopr9823);

  if (thread_sleep(thread2, 3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (thread_exit(thread2, 42) != ERROR_OK)
    TEST_HANG("[thread_exit] error");

  TEST_HANG("unreachable");
}

void			test_core_thread_wait_01(void)
{
  o_thread*		o;
  s_thread_context	ctx;
  s_stack		stack;
  i_cpu			cpu;

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
  ctx.pc = (t_vaddr)test_core_thread_wait_01_thread_01;

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
  ctx.pc = (t_vaddr)test_core_thread_wait_01_thread_02;

  if (thread_load(thread2, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread2) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * scheduler
   */

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(rw98uefskdq0aa);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
