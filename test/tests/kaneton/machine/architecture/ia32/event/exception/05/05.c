/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/event/exception/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2050]
 * updated       julien quintard   [fri jan  7 19:13:12 2011]
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
static volatile t_uint16	gs = (16 << 3);

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

  if (architecture_gdt_delete(16) != ERROR_OK)
    TEST_ERROR("[architecture_gdt_delete] error");

  if (event_reserve(13,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_exception_05_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  asm volatile("movw %0, %%gs"
	       :
	       : "m" (gs));

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  TEST_SIGNATURE(90fiwwmsi93fghgf);

  TEST_LEAVE();
}
