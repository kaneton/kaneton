/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tecture/ia32/educational/event/irq/irq.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat feb  5 21:44:34 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "irq.h"

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

void			test_architecture_event_irq_handler(t_id	id)
{
  thrown++;
}

void			test_architecture_event_irq(void)
{
  int			i;

  if (event_reserve(ARCHITECTURE_IDT_IRQ_FLOPPY,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_irq_handler),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_HANG("[event_reserve] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

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

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  if (thrown != 2)
    TEST_HANG("one or more IRQs have not been caught");

  TEST_SIGNATURE(ber9gsjc93r2g);

  TEST_LEAVE();
}
