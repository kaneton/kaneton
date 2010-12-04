/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/scheduler/context/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri dec  3 22:26:34 2010]
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
extern t_init*			_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_uint32	executed = 0;
static volatile t_uint32	eflags;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_scheduler_context_01_thread (void);
asm ("test_architecture_scheduler_context_01_thread:\n"
     "  cli\n"
     "  pushl %eax\n"
     "  pushf\n"
     "  popl %eax\n"
     "  mov %eax, eflags\n"
     "  popl %eax\n"
     "  movl $1, executed\n"
     "  sti\n"
     "1:\n"
     "  jmp 1b");

void			test_architecture_scheduler_context_01_content(void)
{
  i_thread		thread;
  t_thread_context	ctx;
  t_stack		stack;
  t_clock		clock;
  t_uint64		start;
  o_thread*		t;

  /*
   * thread
   */

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_architecture_scheduler_context_01_thread;

  if (thread_load(thread, ctx) != ERROR_OK)
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

  if (executed != 1)
    TEST_HANG("the thread has not been scheduled");

  if (eflags & (1 << 0))
    TEST_HANG("the eflags CF bit should not be set");

  if (eflags & (1 << 2))
    TEST_HANG("the eflags PF bit should not be set");

  if (eflags & (1 << 6))
    TEST_HANG("the eflags ZF bit should not be set");

  if (eflags & (1 << 7))
    TEST_HANG("the eflags SF bit should not be set");

  if (eflags & (1 << 11))
    TEST_HANG("the eflags OF bit should not be set");

  TEST_SIGNATURE(nsjcwo3f9o0wgkerhg);

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_architecture_scheduler_context_01(void)
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
  ctx.pc = (t_vaddr)test_architecture_scheduler_context_01_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(mvvwif90wfjoag90gh);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
