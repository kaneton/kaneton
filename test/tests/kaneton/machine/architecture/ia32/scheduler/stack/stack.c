/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/scheduler/stack/stack.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:22:09 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "stack.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_vaddr		esp1 = 0;
static volatile t_vaddr		esp2 = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_scheduler_stack_thread_01 (void);
asm ("test_architecture_scheduler_stack_thread_01:\n"
     "  cli\n"
     "  movl %esp, esp1\n"
     "  sti\n"
     "1:\n"
     "  jmp 1b");

void			test_architecture_scheduler_stack_thread_02 (void);
asm ("test_architecture_scheduler_stack_thread_02:\n"
     "  cli\n"
     "  movl %esp, esp2\n"
     "  sti\n"
     "1:\n"
     "  jmp 1b");

void			test_architecture_scheduler_stack_content(void)
{
  i_thread		thread;
  s_thread_context	ctx1;
  s_thread_context	ctx2;
  s_clock		clock;
  t_uint64		start;
  i_cpu			cpu;

  /*
   * thread 1
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_architecture_scheduler_stack_thread_01,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_store(thread, &ctx1) != STATUS_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_architecture_scheduler_stack_thread_02,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

  if (thread_store(thread, &ctx2) != STATUS_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_HANG("[thread_start] error");

  /*
   * sleep
   */

  if (clock_current(&clock) != STATUS_OK)
    TEST_HANG("[clock_current] error");

  start = CLOCK_UNIQUE(&clock);

  while (1)
    {
      t_uint64		current;

      if (clock_current(&clock) != STATUS_OK)
	TEST_HANG("[clock_current] error");

      current = CLOCK_UNIQUE(&clock);

      if (current > (start + 3000))
	break;
    }

  /*
   * check
   */

  if ((esp1 == 0) ||
      (esp2 == 0))
    TEST_HANG("one or both threads have not been scheduled");

  if (((esp1 - 2048) > ctx1.sp) ||
      ((esp2 - 2048) > ctx2.sp))
    TEST_HANG("one of both of the threads' stack seem to be invalid");

  /*
   * scheduler
   */

  TEST_SIGNATURE(f03fj23f09g230g2);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_architecture_scheduler_stack(void)
{
  i_thread		thread;
  i_cpu			cpu;

  /*
   * thread
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_architecture_scheduler_stack_content,
		     &thread) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

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

  TEST_SIGNATURE(nweof32w9ri3gkjehk);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
