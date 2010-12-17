/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...cture/ia32/scheduler/complete/complete.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu dec 16 13:37:53 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "complete.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile i_thread	thread_01;
static volatile t_vaddr		share_01;

static volatile i_task		task_02;
static volatile i_thread	thread_02;
static volatile t_vaddr		share_02;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_scheduler_complete_thread_01(void)
{
  i_cpu			cpu;

  /*
   * sleep
   */

  if (thread_sleep(thread_01, 3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  /*
   * check
   */

  if (*((t_uint32*)(share_01 + 42)) != 1)
    TEST_HANG("the task does not seem to have been executed or "
	      "has an invalid ring level '%u'",
	      *((t_uint32*)(share_01 + 42)));

  /*
   * stop
   */

  TEST_SIGNATURE(scijewf902gkwe9i0);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_architecture_scheduler_complete_thread_02(void)
{
  t_uint32		cs;

  asm volatile("movl %%cs, %0"
               : "=a" (cs));

  *((t_uint32*)(share_02 + 42)) = cs & 0x3;

  while (1)
    ;
}

void			test_architecture_scheduler_complete(void)
{
  i_as			as;
  i_region		region;
  i_segment		segment;
  s_thread_context	ctx;
  s_stack		stack;
  o_thread*		t;
  o_region*		r;
  i_cpu			cpu;

  /*
   * kernel
   */

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &segment) != ERROR_OK)
    TEST_ERROR("[map_reserve] error");

  if (region_reserve(_kernel->as,
		     segment,
		     0,
		     REGION_OPTION_NONE,
		     0x0,
		     PAGESZ,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel->as, region, &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  share_01 = r->address;

  memset((void*)share_01, 0x0, PAGESZ);

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_01) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_01, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread_01, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_architecture_scheduler_complete_thread_01;

  if (thread_load(thread_01, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread_01) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * task
   */

  if (task_reserve(TASK_CLASS_DRIVER,
                   TASK_BEHAVIOUR_INTERACTIVE,
                   TASK_PRIORITY_INTERACTIVE,
                   (i_task*)&task_02) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task_02, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (region_reserve(as,
		     segment,
		     0,
		     REGION_OPTION_NONE,
		     0x0,
		     PAGESZ,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(as, region, &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  share_02 = r->address;

  if (thread_reserve(task_02,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_02) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_02, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread_02, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_architecture_scheduler_complete_thread_02;

  if (thread_load(thread_02, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread_02) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (task_start(task_02) != ERROR_OK)
    TEST_ERROR("[task_start] error");

  /*
   * scheduler
   */

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(nweof32w9ri3gkjehk);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
