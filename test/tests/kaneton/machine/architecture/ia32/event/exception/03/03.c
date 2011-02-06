/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ia32/educational/event/exception/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [sat feb  5 20:56:21 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_exception_03_handler(t_id id)
{
  thrown++;
}

void			test_architecture_event_exception_03(void)
{
  TEST_ENTER();

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_BP,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_exception_03_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  asm volatile("int $3");
  asm volatile("int $3");
  asm volatile("int $3");
  asm volatile("int $3");
  asm volatile("int $3");

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_BP) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  if (thrown != 5)
    TEST_ERROR("one or more exceptions have not been caught");

  TEST_SIGNATURE(3jov9we340g9i3);

  TEST_LEAVE();
}
