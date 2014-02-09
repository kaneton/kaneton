/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/scheduler/context/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [sun jan 30 21:21:55 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_uint32	executed = 0;

static volatile t_uint16	cs;
static volatile t_uint16	ds;
static volatile t_uint16	es;
static volatile t_uint16	fs;
static volatile t_uint16	gs;
static volatile t_uint16	ss;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_scheduler_context_03_thread (void);
asm ("test_architecture_scheduler_context_03_thread:\n"
     "  cli\n"
     "  pushl %eax\n"
     "  movw %cs, %ax\n"
     "  movw %ax, cs\n"
     "  movw %ds, %ax\n"
     "  movw %ax, ds\n"
     "  movw %es, %ax\n"
     "  movw %ax, es\n"
     "  movw %fs, %ax\n"
     "  movw %ax, fs\n"
     "  movw %gs, %ax\n"
     "  movw %ax, gs\n"
     "  movw %ss, %ax\n"
     "  movw %ax, ss\n"
     "  popl %eax\n"
     "  movl $1, executed\n"
     "  sti\n"
     "1:\n"
     "  jmp 1b");

void			test_architecture_scheduler_context_03_content(void)
{
  i_thread		thread;
  s_clock		clock;
  t_uint64		start;
  i_cpu			cpu;

  /*
   * thread
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_architecture_scheduler_context_03_thread,
		     &thread) != STATUS_OK)
    TEST_HANG("[thread_reserve] error");

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

  if (executed != 1)
    TEST_HANG("the thread has not been scheduled");

  if (cs != 0x8)
    TEST_HANG("the CS register is invalid");

  if (ds != 0x10)
    TEST_HANG("the DS register is invalid");

  if (es != 0x10)
    TEST_HANG("the ES register is invalid");

  if (fs != 0x10)
    TEST_HANG("the FS register is invalid");

  if (gs != 0x10)
    TEST_HANG("the GS register is invalid");

  if (ss != 0x10)
    TEST_HANG("the SS register is invalid");

  TEST_SIGNATURE(nsvdiojw0e9w32qfr32);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_architecture_scheduler_context_03(void)
{
  i_thread		thread;
  i_cpu			cpu;

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_architecture_scheduler_context_03_content,
		     &thread) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != STATUS_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(dsnvoweif039g);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
