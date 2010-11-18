/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/event/ia32/exception/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [sun nov 14 23:11:45 2010]
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

void			test_event_ia32_exception_03_handler(t_id	id)
{
  thrown++;
}

void			test_event_ia32_exception_03(void)
{
  TEST_ENTER();

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_exception_03_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("int $3");
  asm volatile("int $3");
  asm volatile("int $3");
  asm volatile("int $3");
  asm volatile("int $3");

  if (event_release(3) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  if (thrown != 5)
    TEST_ERROR("one or more exceptions have not been caught\n");

  TEST_LEAVE();
}
