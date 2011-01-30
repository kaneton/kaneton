/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...achine/architecture/ia32/event/irq/irq.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu jan 27 23:03:28 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "irq.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_irq_handler(t_id	id)
{
  thrown++;
}

void			test_architecture_event_irq_content(void)
{
  int			i;
  i_cpu			cpu;

  if (event_reserve(ARCHITECTURE_IDT_IRQ_FLOPPY,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_irq_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_HANG("[event_reserve] error");

  ARCHITECTURE_OUTB(0x3F0 + 2, 0);
  for (i = 0; i < 10000; i++)
    asm volatile("nop");

  ARCHITECTURE_OUTB(0x3F0 + 2, 0x0C);
  for (i = 0; i < 10000000; i++)
    asm volatile("nop");

  ARCHITECTURE_OUTB(0x3F0 + 2, 0);
  for (i = 0; i < 10000; i++)
    asm volatile("nop");

  ARCHITECTURE_OUTB(0x3F0 + 2, 0x0C);
  for (i = 0; i < 10000000; i++)
    asm volatile("nop");

  if (thrown != 2)
    TEST_HANG("one or more IRQs have not been caught");

  TEST_SIGNATURE(vnmweiofwjf90gg);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_architecture_event_irq(void)
{
  i_thread		thread;
  i_cpu			cpu;

  TEST_ENTER();

  if (thread_reserve(_kernel->task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_architecture_event_irq_content,
		     &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(ber9gsjc93r2g);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
