/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ia32/educational/event/exception/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat feb  5 20:56:03 2011]
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
 
void			test_architecture_event_exception_01_handler(t_id id)
{
  thrown = 1;
}

void			test_architecture_event_exception_01(void)
{
  TEST_ENTER();

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_BP,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_exception_01_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  asm volatile("int $3");

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_BP) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  TEST_SIGNATURE(203i09iwek9fihg93h034);

  TEST_LEAVE();
}
