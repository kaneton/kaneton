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
 * updated       julien quintard   [sat jan 15 16:37:13 2011]
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
  o_thread*		o;
  s_thread_context	ctx;
  s_stack		stack;
  i_cpu			cpu;

  TEST_ENTER();

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_architecture_event_irq_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

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
