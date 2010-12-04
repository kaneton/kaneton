/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/event/exception/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sat dec  4 12:22:07 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_exception_02_handler(t_id id)
{
  thrown = 1;

  if (id != 3)
    TEST_ERROR("invalid event identifier");
}

void			test_architecture_event_exception_02(void)
{
  TEST_ENTER();

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_architecture_event_exception_02_handler),
		    0) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  asm volatile("int $3");

  if (event_release(3) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  TEST_SIGNATURE(2f9023foivvke30gh934);

  TEST_LEAVE();
}
