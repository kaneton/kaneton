/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/task/kernel/kernel.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri dec  3 16:10:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "kernel.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;
extern t_init*			_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_vaddr		share_01;
static volatile t_vaddr		share_02;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_task_kernel_thread_01(void)
{
  if (thread_sleep(3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (*((t_uint32*)share_01) != 42)
    TEST_HANG("the task does not seem to have been executed");

  TEST_SIGNATURE(fwe90gevzdvmcw0egd);

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_task_kernel_thread_02(void)
{
  *((t_uint32*)share_02) = 42;

  while (1)
    ;
}

void			test_core_task_kernel(void)
{
  i_task		task;
  i_thread		thread;
  i_as			as;
  i_region		region;
  i_segment		segment;
  t_thread_context	ctx;
  t_stack		stack;
  t_clock		clock;
  t_uint64		start;
  o_thread*		t;
  o_region*		r;

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

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_task_kernel_thread_01;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * task
   */

  if (task_reserve(TASK_CLASS_KERNEL,
                   TASK_BEHAVIOUR_INTERACTIVE,
                   TASK_PRIORITY_INTERACTIVE,
                   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (region_reserve(as,
                     (i_segment)_init->kcode,
                     0,
                     REGION_OPTION_FORCE | REGION_OPTION_GLOBAL,
                     _init->kcode,
                     _init->kcodesz,
                     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

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

  if (thread_reserve(task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_task_kernel_thread_02;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (task_start(task) != ERROR_OK)
    TEST_ERROR("[task_start] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * scheduler
   */

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(v90wsifc3rfi43g09i34);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
