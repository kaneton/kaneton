/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/core/task/wait/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:17:59 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

#define WAIT_THREAD_SHARED_ADDRESS 0x0FFF1000

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_vaddr		address_00;
static volatile i_thread	thread_00;
static volatile i_thread	thread_01;

static volatile i_task		task_01;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_task_wait_02_thread(void)
{
  volatile t_uint32*	count = (volatile t_uint32*) WAIT_THREAD_SHARED_ADDRESS;

  *count = 0;

  while (1)
    {
      t_uint32		i;

      for (i = 0; i < 123456; i++)
	;

      (*count)++;
    }
}

void			test_core_task_wait_02_content_00(void)
{
  volatile t_uint32*	c = (volatile t_uint32*)address_00;
  t_uint32		count;

  if (task_exit(task_01, 84) != ERROR_OK)
    TEST_HANG("[task_exit] error");

  count = *c;

  if (thread_sleep(thread_00, 3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (count != *c)
    TEST_HANG("the spawn task does not seem to have been exited");

  TEST_SIGNATURE(vn2w30r29gi90hi439h);

  while (1)
    ;
}

void			test_core_task_wait_02_content_01(void)
{
  s_wait		wait;
  i_cpu			cpu;

  if (thread_sleep(thread_01, 3000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (task_wait(thread_01, task_01, WAIT_STATE_DEATH, &wait) != ERROR_OK)
    TEST_HANG("[thread_wait] error");

  if (WAIT_TASK(&wait) != task_01)
    TEST_HANG("invalid waiting task identifier");

  if (WAIT_CAUSE(&wait) != WAIT_STATE_DEATH)
    TEST_HANG("invalid waiting cause");

  if (WAIT_VALUE(&wait) != 84)
    TEST_HANG("invalid waiting value");

  if (thread_sleep(thread_01, 5000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  TEST_SIGNATURE(mwfv09eibv90u2r398);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_task_wait_02(void)
{
  i_as			as;
  i_thread		thread;
  i_cpu			cpu;
  i_segment		segment;
  i_region		region;
  o_region*		r;

  TEST_ENTER();

  /*
   * segment
   */

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &segment) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  /*
   * thread
   */

  if (region_reserve(_kernel.as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     (t_vaddr)NULL,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as,
		 region,
		 &r) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  address_00 = r->address;

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_task_wait_02_content_00,
		     (i_thread*)&thread_00) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread_00) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * thread
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_task_wait_02_content_01,
		     (i_thread*)&thread_01) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");
 
  if (thread_start(thread_01) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (region_reserve(_kernel.as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     (t_vaddr)NULL,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as,
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

  if (region_reserve(as,
		     segment,
		     0x0,
		     REGION_OPTION_FORCE,
		     WAIT_THREAD_SHARED_ADDRESS,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (segment_reserve(as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &segment) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

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

  if (as_copy(_kernel.as, test_core_task_wait_02_thread,
              as, r->address,
              ___kaneton$pagesz) != ERROR_OK)
    TEST_ERROR("[as_copy] error");


  if (thread_reserve(task_01,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     r->address,
		     &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  if (task_start(task_01) != ERROR_OK)
    TEST_ERROR("[task_start] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * scheduler
   */

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(zcmqpok23fweb90ughw3);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
