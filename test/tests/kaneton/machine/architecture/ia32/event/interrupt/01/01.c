/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ia32/educational/event/interrupt/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat feb  5 21:09:08 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_interrupt_01_handler(t_id id)
{
  thrown = 1;

  if (id != (ARCHITECTURE_IDT_SYSCALL_BASE + 0))
    TEST_ERROR("invalid event identifier");
}

void			test_architecture_event_interrupt_01(void)
{
  TEST_ENTER();

  event_release(ARCHITECTURE_IDT_SYSCALL_BASE + 0);

  if (event_reserve(ARCHITECTURE_IDT_SYSCALL_BASE + 0,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_interrupt_01_handler),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

  asm volatile("int $56");

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  if (thrown != 1)
    TEST_ERROR("the interrupt event has not been caught");

  if (event_release(ARCHITECTURE_IDT_SYSCALL_BASE + 0) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(snmvnweg3f90r4geh);

  TEST_LEAVE();
}
