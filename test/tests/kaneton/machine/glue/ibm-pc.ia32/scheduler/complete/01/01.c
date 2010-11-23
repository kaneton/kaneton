/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/ibm-pc.ia32/scheduler/complete/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun nov 21 10:15:49 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task			ktask;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		executed = 0;
static volatile int*		ptr;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ibmpc_ia32_scheduler_complete_01_thread(t_id    id,
								     t_vaddr data)
{
  executed = 1;

  *ptr = 0x5a5a5a5a;

  while (1)
    ;
}

void			test_ibmpc_ia32_scheduler_complete_01_content(void)
{
  i_task		task;
  t_id			thread;
  t_uint32		start;
  o_task*		otask;
  i_as			as;
  o_thread*		othread;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  if (task_reserve(TASK_CLASS_GUEST,
                   TASK_BEHAV_INTERACTIVE,
                   TASK_PRIOR_INTERACTIVE,
                   &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (task_get(task, &otask) != ERROR_NONE)
    TEST_ERROR("[task_get] error\n");

  if (map_reserve(otask->asid,
		  MAP_OPT_NONE,
		  PAGESZ,
		  PERM_READ | PERM_WRITE,
		  (t_vaddr*)&ptr) != ERROR_NONE)
    TEST_ERROR("[map_reserve] error\n");

  if (thread_reserve(task, THREAD_PRIOR, &thread) != ERROR_NONE)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_NONE)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &othread) != ERROR_NONE)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = othread->stack + othread->stacksz - 16;
  ctx.pc = (t_vaddr)test_ibmpc_ia32_scheduler_complete_01_thread;

  if (thread_load(thread, ctx) != ERROR_NONE)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_NONE)
    TEST_ERROR("[ia32_get_context] error\n");

  ia32_ctx.eflags |= (1 << 12);
  ia32_ctx.eflags |= (1 << 13);

  if (ia32_set_context(thread, &ia32_ctx, IA32_CONTEXT_EFLAGS) != ERROR_NONE)
    TEST_ERROR("[ia32_set_context] error\n");

  CLI();
  {
    if (task_state(task, SCHEDULER_STATE_RUN) != ERROR_NONE)
      TEST_ERROR("[task_state] error\n");
  }
  STI();

  start = test_ibmpc_ia32_scheduler_cmos();
  while (((start + 3) % 60) != test_ibmpc_ia32_scheduler_cmos())
    ;

  if (executed != 1)
    TEST_ERROR("the task has not been executed\n");

  while (1)
    ;
}

void			test_ibmpc_ia32_scheduler_complete_01(void)
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
  ctx.pc = (t_vaddr)test_ibmpc_ia32_scheduler_complete_01_content;

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

  printf("1\n");

  STI();     

  printf("2\n");

  CLI();

  printf("3\n");

  TEST_LEAVE();
}
