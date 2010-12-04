/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/scheduler/stack/stack.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri dec  3 22:29:06 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "stack.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;
extern t_init*			_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_vaddr		esp1 = 0;
static volatile t_vaddr		esp2 = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_scheduler_stack_thread_01 (void);
asm ("test_architecture_scheduler_stack_thread_01:\n"
     "  cli\n"
     "  movl %esp, esp1\n"
     "  sti\n"
     "1:\n"
     "  jmp 1b");

void			test_architecture_scheduler_stack_thread_02 (void);
asm ("test_architecture_scheduler_stack_thread_02:\n"
     "  cli\n"
     "  movl %esp, esp2\n"
     "  sti\n"
     "1:\n"
     "  jmp 1b");

void			test_architecture_scheduler_stack_content(void)
{
  i_thread		thread;
  t_thread_context	ctx1;
  t_thread_context	ctx2;
  t_stack		stack;
  o_thread*		t;
  t_clock		clock;
  t_uint64		start;

  /*
   * thread 1
   */

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx1.sp = t->stack + t->stacksz - 16;
  ctx1.pc = (t_vaddr)test_architecture_scheduler_stack_thread_01;

  if (thread_load(thread, ctx1) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx2.sp = t->stack + t->stacksz - 16;
  ctx2.pc = (t_vaddr)test_architecture_scheduler_stack_thread_02;

  if (thread_load(thread, ctx2) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
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

  if ((esp1 == 0) ||
      (esp2 == 0))
    TEST_HANG("one or both threads have not been scheduled");

  if (((esp1 - 2048) > ctx1.sp) ||
      ((esp2 - 2048) > ctx2.sp))
    TEST_HANG("one of both of the threads' stack seem to be invalid");

  /*
   * scheduler
   */

  TEST_SIGNATURE(f03fj23f09g230g2);

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_architecture_scheduler_stack(void)
{
  i_thread		thread;
  t_thread_context	ctx;
  t_stack		stack;
  o_thread*		t;

  /*
   * thread
   */

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_architecture_scheduler_stack_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * scheduler
   */

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(nweof32w9ri3gkjehk);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
