/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ia32/educational/event/exception/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2060]
 * updated       julien quintard   [fri dec 17 14:47:33 2010]
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

void			test_architecture_event_exception_06_handler(t_id id,
								     t_vaddr data)
{
  i_thread		i;
  o_thread*		o;

  if (thread_current(&i) != ERROR_OK)
    TEST_ERROR("[thread_current] error");

  if (thread_get(i, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  thrown = 1;

  if (!(o->machine.error & gs))
    TEST_ERROR("invalid error code");

  gs = 0x10;
}

void			test_architecture_event_exception_06(void)
{
  TEST_ENTER();

  if (event_reserve(13,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_exception_06_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  asm volatile("mov %0, %%gs"
	       :
	       : "m" (gs));

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  if (event_release(13) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(0vfwwfi32w9fwjovkhg);

  TEST_LEAVE();
}
