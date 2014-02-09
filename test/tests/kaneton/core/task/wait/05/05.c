/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/core/task/wait/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:18:19 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

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

void			test_core_task_wait_05_thread(void)
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

void			test_core_task_wait_05_content_00(void)
{
  volatile t_uint32*	c = (volatile t_uint32*)address_00;
  t_uint32		count;

  if (thread_sleep(thread_00, 3000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (task_stop(task_01) != STATUS_OK)
    TEST_ERROR("[task_stop] error");

  count = *c;

  if (thread_sleep(thread_00, 3000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (count != *c)
    TEST_HANG("the spawn task's does not seem to have been stopped");

  TEST_SIGNATURE(cwqf02ig0i3hiyi2f2qf23gg);

  while (1)
    ;
}

void			test_core_task_wait_05_content_01(void)
{
  s_wait		wait;
  i_cpu			cpu;

  if (task_wait(thread_01, task_01, WAIT_STATE_STOP, &wait) != STATUS_OK)
    TEST_HANG("[thread_wait] error");

  if (WAIT_TASK(&wait) != task_01)
    TEST_HANG("invalid waiting task identifier");

  if (WAIT_CAUSE(&wait) != WAIT_STATE_STOP)
    TEST_HANG("invalid waiting cause");

  if (thread_sleep(thread_01, 5000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  TEST_SIGNATURE(xvmwpoiecfpqwkf092i9g0h4h);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_task_wait_05(void)
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
		      &segment) != STATUS_OK)
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
		     &region) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as,
		 region,
		 &r) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  address_00 = r->address;

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_task_wait_05_content_00,
		     (i_thread*)&thread_00) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");
 
  if (thread_start(thread_00) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * thread
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_task_wait_05_content_01,
		     (i_thread*)&thread_01) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");
 
  if (thread_start(thread_01) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  if (region_reserve(_kernel.as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     (t_vaddr)NULL,
		     ___kaneton$pagesz,
		     &region) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as,
		 region,
		 &r) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  address_00 = r->address;

  memset((void*)address_00, 0x0, ___kaneton$pagesz);

  /*
   * task
   */

  if (task_reserve(TASK_CLASS_KERNEL,
		   TASK_BEHAVIOUR_KERNEL,
		   TASK_PRIORITY_KERNEL,
		   (i_task*)&task_01) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task_01, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (region_reserve(as,
		     segment,
		     0x0,
		     REGION_OPTION_FORCE,
		     WAIT_THREAD_SHARED_ADDRESS,
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

  if (as_copy(_kernel.as, (t_vaddr) test_core_task_wait_05_thread,
              as, r->address,
              ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[as_copy] error");

  if (thread_reserve(task_01,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     r->address,
		     &thread) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (task_start(task_01) != STATUS_OK)
    TEST_ERROR("[task_start] error");

  if (thread_start(thread) != STATUS_OK)
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

  TEST_SIGNATURE(zxcmnqwr9gibh094ih03gh03hk);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
