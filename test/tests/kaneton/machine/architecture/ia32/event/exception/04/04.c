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
 * updated       julien quintard   [sun feb  6 16:07:22 2011]
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

void			test_architecture_event_exception_04_handler(t_id id)
{
  thrown = 1;
  div = 1;
}

void			test_architecture_event_exception_04(void)
{
  TEST_ENTER();

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_DE,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_exception_04_handler),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  asm volatile("mov $1, %%eax\n"
               "mov $0, %%edx\n"
               "divl %0"
               :
               : "m" (div)
               : "eax", "edx");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_DE) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(v09i2wkv943gfi3hg);

  TEST_LEAVE();
}
