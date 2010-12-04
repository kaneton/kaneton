/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/event/data/data.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat dec  4 12:24:15 2010]
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

void			test_architecture_event_data_handler(t_id	id,
						     t_vaddr	pv)
{
  thrown = 1;
  
  if (pv != 0x42424242)
    TEST_ERROR("the event data address differs from the one provided "
	       "at the reservation");
}

void			test_architecture_event_data(void)
{
  TEST_ENTER();

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_architecture_event_data_handler),
		    0x42424242) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  asm volatile("int $3");

  if (event_release(3) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  if (thrown != 1)
    TEST_ERROR("the exception does not seem to have been thrown");

  TEST_SIGNATURE(v09iwij90ig4ghh);

  TEST_LEAVE();
}
