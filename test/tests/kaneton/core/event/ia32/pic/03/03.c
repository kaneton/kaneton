/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/event/ia32/pic/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [wed nov 17 08:19:21 2010]
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

void			test_event_ia32_pic_03_handler(t_id		id)
{
}

void			test_event_ia32_pic_03(void)
{
  t_uint8		mask;

  TEST_ENTER();

  if (event_reserve(32 + 3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_pic_03),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  INB(MASTER_PORT_B, mask);

  if ((mask & (1 << 3)) == 0)
    TEST_ERROR("invalid PIC mask\n");

  INB(SLAVE_PORT_B, mask);

  if (mask != 0xff)
    TEST_ERROR("invalid PIC mask\n");

  if (event_release(32 + 3) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  TEST_LEAVE();
}
