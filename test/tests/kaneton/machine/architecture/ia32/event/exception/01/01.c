/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...rchitecture/ia32/event/exception/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [tue nov 30 18:13:05 2010]
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

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_architecture_event_exception_01_handler),
		    0) != ERROR_OK)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("int $3");

  if (event_release(3) != ERROR_OK)
    TEST_ERROR("[event_release] error\n");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught\n");

  TEST_SIGNATURE(203i09iwek9fihg93h034);

  TEST_LEAVE();
}
