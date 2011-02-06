/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...cture/ia32/educational/event/data/data.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat feb  5 21:14:04 2011]
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
		    EVENT_DATA(0x42424242)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  asm volatile("int $3");

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_BP) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  if (thrown != 1)
    TEST_ERROR("the exception does not seem to have been thrown");

  TEST_SIGNATURE(v09iwij90ig4ghh);

  TEST_LEAVE();
}
