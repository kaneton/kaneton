/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/event/ia32/interrupt/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [tue nov 16 20:49:41 2010]
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

void			test_event_ia32_interrupt_01_handler(t_id		id)
{
  thrown = 1;

  if (id != 56)
    TEST_ERROR("invalid event identifier");
}

void			test_event_ia32_interrupt_01(void)
{
  TEST_ENTER();

  event_release(56);

  if (event_reserve(56,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_interrupt_01_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("int $56");

  if (thrown != 1)
    TEST_ERROR("the interrupt event has not been caught\n");

  TEST_LEAVE();
}
