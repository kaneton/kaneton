/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/glue/ibm-pc.ia32/task/clone/clone.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun nov 21 09:56:48 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clone.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		executed = 0;
static volatile int		go = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ibmpc_ia32_task_clone_thread(void)
{
  CLI();
  {
    executed++;
  }
  STI();

  while (1)
    {
      if ((go == 1) && (executed == 1))
	executed++;
    }
}

void			test_ibmpc_ia32_task_clone_content(void)
{
  i_task		tsk;
  i_task		cloned;
  t_id			thread;
  t_uint32		start;
  i_as			as;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  if (task_reserve(TASK_CLASS_DRIVER,
                   TASK_BEHAVIOUR_INTERACTIVE,
                   TASK_PRIORITY_INTERACTIVE,
                   &tsk) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(tsk, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error\n");

  if (thread_reserve(tsk, THREAD_PRIOR, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_ibmpc_ia32_task_clone_thread;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_OK)
    TEST_ERROR("[ia32_get_context] error\n");

  ia32_ctx.eflags |= (1 << 12);
  ia32_ctx.eflags |= (1 << 13);

  if (ia32_set_context(thread, &ia32_ctx, IA32_CONTEXT_EFLAGS) != ERROR_OK)
    TEST_ERROR("[ia32_set_context] errorn");

  if (scheduler_add(thread) != ERROR_OK)
    TEST_ERROR("[scheduler_add] error\n");

  CLI();
  {
    if (task_state(tsk, SCHEDULER_STATE_RUN) != ERROR_OK)
      TEST_ERROR("[task_state] error: unable to set the task as running\n");
  }
  STI();

  start = test_ibmpc_ia32_task_cmos();
  while (start == test_ibmpc_ia32_task_cmos())
    ;

  if (executed != 1)
    TEST_ERROR("the thread has not been executed\n");

  go = 1;

  CLI();
  {
    if (task_clone(tsk, &cloned) != ERROR_OK)
      TEST_ERROR("[task_clone] error\n");
  }
  STI();

  start = test_ibmpc_ia32_task_cmos();
  while (start == test_ibmpc_ia32_task_cmos())
    ;

  if (executed != 2)
    TEST_ERROR("the cloned thread has not been executed\n");

  while (1)
    ;
}

void			test_ibmpc_ia32_task_clone(void)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  TEST_ENTER();

  if (thread_reserve(_kernel->task, THREAD_PRIOR, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_ibmpc_ia32_task_clone_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_OK)
    TEST_ERROR("[ia32_get_context] error\n");

  ia32_ctx.eflags |= (1 << 12);
  ia32_ctx.eflags |= (1 << 13);

  if (ia32_set_context(thread, &ia32_ctx, IA32_CONTEXT_EFLAGS) != ERROR_OK)
    TEST_ERROR("[ia32_set_context] errorn");

  if (scheduler_add(thread) != ERROR_OK)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  TEST_LEAVE();
}
