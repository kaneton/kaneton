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
 * updated       julien quintard   [thu jan 27 22:29:36 2011]
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
  i_cpu			cpu;

  /*
   * thread
   */

  if (thread_reserve(task_01,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
		     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_complete_02_thread_03,
		     (i_thread*)&thread_03) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_start(thread_03) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread
   */

  if (thread_reserve(task_02,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_complete_02_thread_05,
		     (i_thread*)&thread_05) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

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

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
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
  o_region*		r;
  i_cpu			cpu;

  /*
   * kernel
   */

  if (segment_reserve(_kernel->as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &segment) != ERROR_OK)
    TEST_ERROR("[map_reserve] error");

  if (region_reserve(_kernel->as,
		     segment,
		     0,
		     REGION_OPTION_NONE,
		     0x0,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel->as, region, &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  share_01 = r->address;

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_complete_02_thread_01,
		     (i_thread*)&thread_01) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

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
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(as, region, &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  share_02 = r->address;

  if (thread_reserve(task_01,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_complete_02_thread_02,
		     (i_thread*)&thread_02) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

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
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(as, region, &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  share_03 = r->address;

  if (thread_reserve(task_02,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_scheduler_complete_02_thread_04,
		     (i_thread*)&thread_04) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread_04) != ERROR_OK)
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

  TEST_SIGNATURE(zdvcowejfwegbeihbe0);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
