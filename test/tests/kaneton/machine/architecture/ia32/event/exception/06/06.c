/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/event/exception/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2060]
 * updated       julien quintard   [sun nov 21 00:05:33 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		thrown = 0;
static volatile t_uint16	gs = (16 << 3);

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ia32_event_exception_06_handler(t_id	id,
							     t_vaddr	address,
							     t_uint32	error)
{
  thrown = 1;

  if (!(error & gs))
    TEST_ERROR("invalid error code\n");

  gs = 0x10;
}

void			test_ia32_event_exception_06(void)
{
  TEST_ENTER();

  if (event_reserve(13,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_ia32_event_exception_06_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("mov %0, %%gs"
	       :
	       : "m" (gs));

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught\n");

  if (event_release(13) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  TEST_LEAVE();
}
