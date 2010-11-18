/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/event/ia32/data/data.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun nov 14 23:10:52 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "data.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_event_data_handler(t_id		id,
						t_vaddr		pv)
{
  thrown = 1;
  
  if (pv != 0x42424242)
    TEST_ERROR("the event data address differs from the one provided "
	       "at the reservation\n");
}

void			test_event_data(void)
{
  TEST_ENTER();

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_data_handler),
		    0x42424242) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("int $3");

  if (event_release(3) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  if (thrown != 1)
    TEST_ERROR("the exception does not seem to have been thrown\n");

  TEST_LEAVE();
}
