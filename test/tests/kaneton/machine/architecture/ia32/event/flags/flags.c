/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/architecture/ia32/event/flags/flags.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:28:09 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "flags.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_flags_handler(t_id	id)
{
  t_uint32		eflags;

  thrown = 1;

  asm volatile("pushf\n"
               "popl %0"
               : "=g" (eflags));

  TEST_PRINT("[isr] eflags: %s\n",
	 (eflags & (1 << 9) ? "enabled" : "disabled"));
}

void			test_architecture_event_flags_content(void)
{
  t_uint32		eflags;
  i_cpu			cpu;

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_BP,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_flags_handler),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_HANG("[event_reserve] error");

  asm volatile("int $3");

  if (thrown != 1)
    TEST_HANG("the exception has not been caught");

  asm volatile("pushf\n"
               "popl %0"
               : "=g" (eflags));

  TEST_PRINT("[content] eflags: %s\n",
	 (eflags & (1 << 9) ? "enabled" : "disabled"));

  TEST_SIGNATURE(09i2309if09jgjhh3);

  if (cpu_current(&cpu) != STATUS_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != STATUS_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_architecture_event_flags(void)
{
  i_thread		thread;
  i_cpu			cpu;

  TEST_ENTER();

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_architecture_event_flags_content,
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

  TEST_SIGNATURE(vsmnefi90g4h00);

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
