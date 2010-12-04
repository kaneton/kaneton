/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ts/kaneton/core/thread/reserve/reserve.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat dec  4 13:18:58 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "reserve.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

void			test_core_thread_reserve_content(void)
{
  TEST_HANG("unreachable");
}

void			test_core_thread_reserve(void)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;

  TEST_ENTER();

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_thread_reserve_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  TEST_SIGNATURE(r2390fiw390ggh4r);

  TEST_LEAVE();
}
