/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/core/task/service/service.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:17:10 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "service.h"

#define SERVICE_SHARED_ADDRESS 0x0FFF1000

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile i_thread	thread_01;
static volatile t_vaddr		share_01;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_task_service_thread_01(void)
{
  i_cpu			cpu;

  if (thread_sleep(thread_01, 3000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (*((t_uint32*)share_01) != 42)
    TEST_HANG("the task does not seem to have been executed");

  TEST_SIGNATURE(90cwscfmwefw32g9f0);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_task_service_thread_02(void)
{
  *((t_uint32*) SERVICE_SHARED_ADDRESS) = 42;

  while (1)
    ;
}

void			test_core_task_service(void)
{
  i_thread              thread_02;
  i_task		task;
  i_as			as;
  i_region		region;
  i_segment		segment;
  o_region*		r;
  i_cpu			cpu;

  /*
   * kernel
   */

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &segment) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
		     segment,
		     0,
		     REGION_OPTION_NONE,
		     0x0,
		     ___kaneton$pagesz,
		     &region) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as, region, &r) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  share_01 = r->address;

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_task_service_thread_01,
		     (i_thread*)&thread_01) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread_01) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * task
   */

  if (task_reserve(TASK_CLASS_SERVICE,
                   TASK_BEHAVIOUR_INTERACTIVE,
                   TASK_PRIORITY_INTERACTIVE,
                   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (region_reserve(as,
		     segment,
		     0,
		     REGION_OPTION_FORCE,
		     SERVICE_SHARED_ADDRESS,
		     ___kaneton$pagesz,
		     &region) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (segment_reserve(as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &segment) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
                     segment,
                     0,
                     REGION_OPTION_NONE,
                     0x0,
                     ___kaneton$pagesz,
                     &region) != STATUS_OK)
      TEST_ERROR("[region_reserve] error");

  if (region_get(as, region, &r) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  if (as_copy(_kernel.as, (t_vaddr) test_core_task_service_thread_02,
              as, r->address,
              ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[as_copy] error");

  if (thread_reserve(task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     r->address,
		     (i_thread*)&thread_02) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (task_start(task) != STATUS_OK)
    TEST_ERROR("[task_start] error");

  if (thread_start(thread_02) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * scheduler
   */

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != STATUS_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(veofo23w9fri92309gig);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
