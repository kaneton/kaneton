/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../kaneton/core/scheduler/complete/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri dec  3 22:06:52 2010]
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

static volatile t_vaddr		share_01;

static volatile i_task		task_02;
static volatile t_vaddr		share_02;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_scheduler_complete_01_thread_01(void)
{
  i_thread		thread;
  t_thread_context	ctx;
  t_stack		stack;
  o_thread*		t;

  /*
   * sleep
   */

  if (thread_sleep(3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  /*
   * check
   */

  if (*((t_uint32*)share_01) != 42)
    TEST_HANG("the task does not seem to have been executed");

  /*
   * thread
   */

  if (thread_reserve(task_02, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_complete_01_thread_03;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * sleep
   */

  if (thread_sleep(3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  /*
   * check
   */

  if (*((t_uint32*)(share_01 + 2042)) != 84)
    TEST_HANG("the thread does not seem to have been executed");

  /*
   * stop
   */

  TEST_SIGNATURE(weiofjwc39fw30gi99);

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_scheduler_complete_01_thread_02(void)
{
  *((t_uint32*)share_02) = 42;

  while (1)
    ;
}

void			test_core_scheduler_complete_01_thread_03(void)
{
  *((t_uint32*)(share_02 + 2042)) = 84;

  while (1)
    ;
}

void			test_core_scheduler_complete_01(void)
{
  i_thread		thread;
  i_as			as;
  i_region		region;
  i_segment		segment;
  t_thread_context	ctx;
  t_stack		stack;
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
  ctx.pc = (t_vaddr)test_core_scheduler_complete_01_thread_01;

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
                   (i_task*)&task_02) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task_02, &as) != ERROR_OK)
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

  if (thread_reserve(task_02, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_complete_01_thread_02;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (task_start(task_02) != ERROR_OK)
    TEST_ERROR("[task_start] error");

  /*
   * scheduler
   */

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(ccwseifvwb09giifawooierwg);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
