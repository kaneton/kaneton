/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../kaneton/core/scheduler/complete/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun dec  5 00:54:16 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile i_thread	thread_01;
static volatile t_vaddr		share_01;

static volatile i_task		task_01;
static volatile i_thread	thread_02;
static volatile i_thread	thread_03;
static volatile t_vaddr		share_02;

static volatile i_task		task_02;
static volatile i_thread	thread_04;
static volatile i_thread	thread_05;
static volatile t_vaddr		share_03;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_scheduler_complete_02_thread_01(void)
{
  t_thread_context	ctx;
  t_stack		stack;
  o_thread*		t;

  /*
   * thread
   */

  if (thread_reserve(task_01,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_03) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_03, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread_03, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_complete_02_thread_03;

  if (thread_load(thread_03, ctx) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread_03) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread
   */

  if (thread_reserve(task_02,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_05) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_05, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread_05, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_complete_02_thread_05;

  if (thread_load(thread_05, ctx) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread_05) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * sleep
   */

  if (thread_sleep(thread_01, 3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  /*
   * check
   */

  if (*((t_uint32*)(share_01 + 20)) != 20)
    TEST_HANG("a thread seem not to have been executed");

  if (*((t_uint32*)(share_01 + 40)) != 40)
    TEST_HANG("a thread seem not to have been executed");

  if (*((t_uint32*)(share_01 + 60)) != 60)
    TEST_HANG("a thread seem not to have been executed");

  if (*((t_uint32*)(share_01 + 80)) != 80)
    TEST_HANG("a thread seem not to have been executed");

  /*
   * stop
   */

  TEST_SIGNATURE(23rfjsovcewrnbe04);

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_scheduler_complete_02_thread_02(void)
{
  *((t_uint32*)(share_02 + 20)) = 20;

  while (1)
    ;
}

void			test_core_scheduler_complete_02_thread_03(void)
{
  *((t_uint32*)(share_02 + 40)) = 40;

  while (1)
    ;
}

void			test_core_scheduler_complete_02_thread_04(void)
{
  *((t_uint32*)(share_03 + 60)) = 60;

  while (1)
    ;
}

void			test_core_scheduler_complete_02_thread_05(void)
{
  *((t_uint32*)(share_03 + 80)) = 80;

  while (1)
    ;
}

void			test_core_scheduler_complete_02(void)
{
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
  ctx.pc = (t_vaddr)test_core_scheduler_complete_02_thread_01;

  if (thread_load(thread_01, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread_01) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * task 1
   */

  if (task_reserve(TASK_CLASS_KERNEL,
                   TASK_BEHAVIOUR_INTERACTIVE,
                   TASK_PRIORITY_INTERACTIVE,
                   (i_task*)&task_01) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task_01, &as) != ERROR_OK)
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

  if (thread_reserve(task_01,
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
  ctx.pc = (t_vaddr)test_core_scheduler_complete_02_thread_02;

  if (thread_load(thread_02, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread_02) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (task_start(task_01) != ERROR_OK)
    TEST_ERROR("[task_start] error");

  /*
   * task 3
   */

  if (task_reserve(TASK_CLASS_KERNEL,
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

  share_03 = r->address;

  if (thread_reserve(task_02,
		     THREAD_PRIORITY,
		     (i_thread*)&thread_04) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread_04, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread_04, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_core_scheduler_complete_02_thread_04;

  if (thread_load(thread_04, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread_04) != ERROR_OK)
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

  TEST_SIGNATURE(zdvcowejfwegbeihbe0);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
