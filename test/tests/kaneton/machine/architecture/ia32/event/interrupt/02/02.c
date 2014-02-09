/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ia32/educational/event/interrupt/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sat feb  5 21:09:26 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_interrupt_02_handler_56(t_id id)
{
  thrown++;

  if (id != (ARCHITECTURE_IDT_SYSCALL_BASE + 0))
    TEST_ERROR("invalid event identifier");
}

void			test_architecture_event_interrupt_02_handler_57(t_id id)
{
  thrown++;

  if (id != (ARCHITECTURE_IDT_SYSCALL_BASE + 1))
    TEST_ERROR("invalid event identifier");
}

void			test_architecture_event_interrupt_02_handler_58(t_id id)
{
  thrown++;

  if (id != (ARCHITECTURE_IDT_SYSCALL_BASE + 2))
    TEST_ERROR("invalid event identifier");
}

void			test_architecture_event_interrupt_02(void)
{
  TEST_ENTER();

  event_release(ARCHITECTURE_IDT_SYSCALL_BASE + 0);
  event_release(ARCHITECTURE_IDT_SYSCALL_BASE + 1);
  event_release(ARCHITECTURE_IDT_SYSCALL_BASE + 2);

  if (event_reserve(ARCHITECTURE_IDT_SYSCALL_BASE + 0,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_interrupt_02_handler_56),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_reserve(ARCHITECTURE_IDT_SYSCALL_BASE + 1,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_interrupt_02_handler_57),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_reserve(ARCHITECTURE_IDT_SYSCALL_BASE + 2,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_interrupt_02_handler_58),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

  asm volatile("int $56");
  asm volatile("int $57");
  asm volatile("int $58");

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  if (thrown != 3)
    TEST_ERROR("some interrupt events have not been caught");

  if (event_release(ARCHITECTURE_IDT_SYSCALL_BASE + 0) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  if (event_release(ARCHITECTURE_IDT_SYSCALL_BASE + 1) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  if (event_release(ARCHITECTURE_IDT_SYSCALL_BASE + 2) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(cd09iwfjg9gerkg43hg);

  TEST_LEAVE();
}
