/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/core/task/sleep/sleep.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:17:23 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "sleep.h"

#define SLEEP_THREAD_SHARED_ADDRESS 0x0FFF1000

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_vaddr		address_00;
static volatile i_thread	thread_00;

static volatile i_task		task_01;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_task_sleep_thread(void)
{
  volatile t_uint32*	count = (volatile t_uint32*) SLEEP_THREAD_SHARED_ADDRESS;

  *count = 0;

  while (1)
    {
      t_uint32		i;

      for (i = 0; i < 123456; i++)
	;

      (*count)++;
    }
}

void			test_core_task_sleep_content(void)
{
  volatile t_uint32*	c = (volatile t_uint32*)address_00;
  i_cpu			cpu;
  t_uint32		count;

  count = *c;

  if (thread_sleep(thread_00, 3000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (count >= *c)
    TEST_HANG("the spawn task's thread does not seem to have been executed");

  if (task_sleep(task_01, 5000) != STATUS_OK)
    TEST_HANG("[task_sleep] error");

  count = *c;

  if (thread_sleep(thread_00, 3000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (count != *c)
    TEST_HANG("the spawn task has not been put to sleep or woken up too soon");

  if (thread_sleep(thread_00, 4000) != STATUS_OK)
    TEST_HANG("[thread_sleep] error");

  if (count >= *c)
    TEST_HANG("the spawn task has not been woken up as expected");

  TEST_SIGNATURE(f0w9jq23fojg309hg34);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_task_sleep(void)
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

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_task_sleep_content,
		     (i_thread*)&thread_00) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");
 
  if (thread_start(thread_00) != STATUS_OK)
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
		     SLEEP_THREAD_SHARED_ADDRESS,
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

  if (as_copy(_kernel.as, (t_vaddr) test_core_task_sleep_thread,
              as, r->address, ___kaneton$pagesz) != STATUS_OK)
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

  TEST_SIGNATURE(niowef02r9nvoawvv);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
