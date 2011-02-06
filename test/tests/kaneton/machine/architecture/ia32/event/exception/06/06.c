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
 * updated       julien quintard   [sat feb  5 21:05:26 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern am			_architecture;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		thrown = 0;
static volatile t_uint16	gs = (0x10 << 3);

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_exception_06_handler(t_id id,
								     t_vaddr data)
{
  thrown = 1;

  if (!(_architecture.error & gs))
    TEST_ERROR("invalid error code");

  gs = 0x10;
}

void			test_architecture_event_exception_06(void)
{
  TEST_ENTER();

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_GP,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_exception_06_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  asm volatile("mov %0, %%gs"
	       :
	       : "m" (gs));

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_GP) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(0vfwwfi32w9fwjovkhg);

  TEST_LEAVE();
}
