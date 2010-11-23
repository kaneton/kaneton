/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/event/pic/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun nov 21 00:14:08 2010]
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

void			test_ia32_event_pic_01_handler(t_id		id)
{
}

void			test_ia32_event_pic_01(void)
{
  t_uint8		mask;

  TEST_ENTER();

  if (event_reserve(32 + 6,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_ia32_event_pic_01),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  INB(MASTER_PORT_B, mask);

  if ((mask & (1 << 6)) != 0)
    TEST_ERROR("invalid PIC mask\n");

  INB(SLAVE_PORT_B, mask);

  if (mask != 0xff)
    TEST_ERROR("invalid PIC mask\n");

  TEST_LEAVE();
}
