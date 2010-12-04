/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/event/interrupt/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat dec  4 12:26:36 2010]
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

  if (id != 56)
    TEST_ERROR("invalid event identifier");
}

void			test_architecture_event_interrupt_01(void)
{
  TEST_ENTER();

  event_release(56);

  if (event_reserve(56,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_architecture_event_interrupt_01_handler),
		    0) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  asm volatile("int $56");

  if (thrown != 1)
    TEST_ERROR("the interrupt event has not been caught");

  if (event_release(56) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(snmvnweg3f90r4geh);

  TEST_LEAVE();
}
