/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/glue/ibm-pc.ia32/task/driver/driver.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sat nov 20 23:10:32 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "driver.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task			ktask;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		executed = 0;
static volatile int		esp = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ibmpc_ia32_task_driver_thread(void)
{
  t_uint32		cs;

  asm volatile("movl %%esp, esp\n"
               "movl %%cs, %0"
               : "=a" (cs));

  printf("[ring] %d\n", cs & 0x3);

  executed = 1;

  while (1)
    ;
}

void			test_ibmpc_ia32_task_driver_content(void)
{
  i_task		tsk;
  t_id			thread;
  t_uint32		start;
  i_as			as;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  if (task_reserve(TASK_CLASS_DRIVER,
                   TASK_BEHAV_INTERACTIVE,
                   TASK_PRIOR_INTERACTIVE,
                   &tsk) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(tsk, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (thread_reserve(tsk, THREAD_PRIOR, &thread) != ERROR_NONE)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_NONE)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_NONE)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_ibmpc_ia32_task_driver_thread;

  if (thread_load(thread, ctx) != ERROR_NONE)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_NONE)
    TEST_ERROR("[ia32_get_context] error\n");

  ia32_ctx.eflags |= (1 << 12);
  ia32_ctx.eflags |= (1 << 13);

  if (ia32_set_context(thread, &ia32_ctx, IA32_CONTEXT_EFLAGS) != ERROR_NONE)
    TEST_ERROR("[ia32_set_context] errorn");

  if (scheduler_add(thread) != ERROR_NONE)
    TEST_ERROR("[scheduler_add] error\n");

  CLI();
  {
    if (task_state(tsk, SCHEDULER_STATE_RUN) != ERROR_NONE)
      TEST_ERROR("[task_state] error: unable to set the task as running\n");
  }
  STI();

  start = test_ibmpc_ia32_task_cmos();
  while (start == test_ibmpc_ia32_task_cmos())
    ;

  if (executed != 1)
    TEST_ERROR("the thread has not been executed\n");

  if (thread_store(thread, &ctx) != ERROR_NONE)
    TEST_ERROR("[thread_store] error\n");

  if ((ctx.sp + 4096 > esp) && (ctx.sp - 4096 < esp))
    TEST_ERROR("invalid stack\n");

  while (1)
    ;
}

void			test_ibmpc_ia32_task_driver(void)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  TEST_ENTER();

  if (thread_reserve(ktask, THREAD_PRIOR, &thread) != ERROR_NONE)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_NONE)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_NONE)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_ibmpc_ia32_task_driver_content;

  if (thread_load(thread, ctx) != ERROR_NONE)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_NONE)
    TEST_ERROR("[ia32_get_context] error\n");

  ia32_ctx.eflags |= (1 << 12);
  ia32_ctx.eflags |= (1 << 13);

  if (ia32_set_context(thread, &ia32_ctx, IA32_CONTEXT_EFLAGS) != ERROR_NONE)
    TEST_ERROR("[ia32_set_context] errorn");

  if (scheduler_add(thread) != ERROR_NONE)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  TEST_LEAVE();
}
