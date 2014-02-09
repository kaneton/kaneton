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
 * updated       julien quintard   [sun feb  6 16:06:48 2011]
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

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_BP,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_data_handler),
		    EVENT_DATA(0x42424242)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  asm volatile("int $3");

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_BP) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  if (thrown != 1)
    TEST_ERROR("the exception does not seem to have been thrown");

  TEST_SIGNATURE(v09iwij90ig4ghh);

  TEST_LEAVE();
}
