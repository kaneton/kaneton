/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/event/exception/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2040]
 * updated       julien quintard   [sun nov 21 00:03:25 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;
static volatile int	div = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ia32_event_exception_04_handler(t_id	id)
{
  thrown = 1;
  div = 1;
}

void			test_ia32_event_exception_04(void)
{
  TEST_ENTER();

  if (event_reserve(0,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_ia32_event_exception_04_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("mov $1, %%eax\n"
               "mov $0, %%edx\n"
               "divl %0"
               :
               : "m" (div)
               : "eax", "edx");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught\n");

  if (event_release(0) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  TEST_LEAVE();
}
