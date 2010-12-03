/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...rchitecture/ia32/event/exception/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2050]
 * updated       julien quintard   [tue nov 30 18:13:47 2010]
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

  if (ia32_gdt_delete_segment(NULL, 16) != ERROR_OK)
    TEST_ERROR("[ia32_gdt_delete_segment] error\n");

  if (event_reserve(13,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_architecture_event_exception_05_handler),
		    0) != ERROR_OK)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("movw %0, %%gs"
	       :
	       : "m" (gs));

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught\n");

  TEST_SIGNATURE(90fiwwmsi93fghgf);

  TEST_LEAVE();
}
