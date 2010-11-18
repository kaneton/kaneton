/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/event/ia32/interrupt/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [wed nov 17 08:14:41 2010]
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

void			test_event_ia32_interrupt_02_handler_56(t_id		id)
{
  thrown++;

  if (id != 56)
    TEST_ERROR("invalid event identifier");
}

void			test_event_ia32_interrupt_02_handler_57(t_id		id)
{
  thrown++;

  if (id != 57)
    TEST_ERROR("invalid event identifier");
}

void			test_event_ia32_interrupt_02_handler_58(t_id		id)
{
  thrown++;

  if (id != 58)
    TEST_ERROR("invalid event identifier");
}

void			test_event_ia32_interrupt_02(void)
{
  TEST_ENTER();

  event_release(56);
  event_release(57);
  event_release(58);

  if (event_reserve(56,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_interrupt_02_handler_56),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  if (event_reserve(57,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_interrupt_02_handler_57),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  if (event_reserve(58,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_interrupt_02_handler_58),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("int $56");
  asm volatile("int $57");
  asm volatile("int $58");

  if (thrown != 3)
    TEST_ERROR("some interrupt events have not been caught\n");

  if (event_release(56) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  if (event_release(57) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  if (event_release(58) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  TEST_LEAVE();
}
