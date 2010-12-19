/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/core/task/wait/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:06 2010]
 * updated       julien quintard   [sat dec 18 22:38:57 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_vaddr		address_00;
static volatile i_thread	thread_00;
static volatile i_thread	thread_01;

static volatile i_task		task_01;
static volatile t_vaddr		address_01;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_task_wait_06_thread(void)
{
  volatile t_uint32*	count = (volatile t_uint32*)address_01;

  *count = 0;

  while (1)
    {
      t_uint32		i;

      for (i = 0; i < 123456; i++)
	;

      (*count)++;
    }
}

void			test_core_task_wait_06_content_00(void)
{
  volatile t_uint32*	c = (volatile t_uint32*)address_00;
  t_uint32		count;

  if (task_stop(task_01) != ERROR_OK)
    TEST_ERROR("[task_start] error");

  count = *c;

  if (thread_sleep(thread_00, 3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (count != *c)
    TEST_HANG("the spawn task's does not seem to have been stopped");

  TEST_SIGNATURE(vcnwerf20fgh30hj0hb02);

  while (1)
    ;
}

void			test_core_task_wait_06_content_01(void)
{
  s_wait		wait;
  i_cpu			cpu;

  if (thread_sleep(thread_01, 3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (task_wait(thread_01, task_01, WAIT_STATE_STOP, &wait) != ERROR_OK)
    TEST_HANG("[thread_wait] error");

  if (WAIT_TASK(&wait) != task_01)
    TEST_HANG("invalid waiting task identifier");

  if (WAIT_CAUSE(&wait) != WAIT_STATE_STOP)
    TEST_HANG("invalid waiting cause");

  if (thread_sleep(thread_01, 5000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  TEST_SIGNATURE(zxcmq3rj20g90ihb90ihy);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_task_wait_06(void)
{
  i_as			as;
  i_thread		thread;
  o_thread*		o;
  s_thread_context	ctx;
  s_stack		stack;
  i_cpu			cpu;
  i_segment		segment;
  i_region		region;
  o_region*		r;

  TEST_ENTER();

  /*
   * segment
   */

  if (segment_reserve(_kernel->as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &segment) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  /*
   * thread
   */

  if (region_reserve(_kernel->as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     (t_vaddr)NULL,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel->as,
		 region,
		 &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  address_00 = r->address;

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_00) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");
 
  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_00, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread_00, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_task_wait_06_content_00;

  if (thread_load(thread_00, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread_00) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * thread
   */

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_01) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");
 
  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_01, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread_01, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_task_wait_06_content_01;

  if (thread_load(thread_01, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread_01) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (region_reserve(_kernel->as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     (t_vaddr)NULL,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel->as,
		 region,
		 &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  address_00 = r->address;

  memset((void*)address_00, 0x0, ___kaneton$pagesz);

  /*
   * task
   */

  if (task_reserve(TASK_CLASS_KERNEL,
		   TASK_BEHAVIOUR_KERNEL,
		   TASK_PRIORITY_KERNEL,
		   (i_task*)&task_01) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task_01, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (thread_reserve(task_01,
		     THREAD_PRIORITY,
		     &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_task_wait_06_thread;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (task_start(task_01) != ERROR_OK)
    TEST_ERROR("[task_start] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (region_reserve(as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     (t_vaddr)NULL,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(as,
		 region,
		 &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  address_01 = r->address;

  /*
   * scheduler
   */

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(ncuwqdq23jd92jf093gu);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
