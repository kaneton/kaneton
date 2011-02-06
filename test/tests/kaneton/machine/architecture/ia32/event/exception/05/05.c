/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ia32/educational/event/exception/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2050]
 * updated       julien quintard   [sat feb  5 20:56:55 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		thrown = 0;
static volatile t_uint16	gs = (0x10 << 3);

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_exception_05_handler(t_id id,
								     t_vaddr address,
								     t_uint32 error)
{
  thrown = 1;

  gs = 0x10;
}

void			test_architecture_event_exception_05(void)
{
  TEST_ENTER();

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_GP,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_exception_05_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  asm volatile("movw %0, %%gs"
	       :
	       : "m" (gs));

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  TEST_SIGNATURE(90fiwwmsi93fghgf);

  TEST_LEAVE();
}
