/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/event/pic/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sun nov 21 00:14:42 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ia32_event_pic_02_handler(t_id		id)
{
}

void			test_ia32_event_pic_02(void)
{
  t_uint8		mask;

  TEST_ENTER();

  if (event_reserve(32 + 9,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_ia32_event_pic_02),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  INB(MASTER_PORT_B, mask);

  if ((mask & (1 << 2)) != 0)
    TEST_ERROR("invalid PIC mask\n");

  INB(SLAVE_PORT_B, mask);

  if (mask != 0xfd)
    TEST_ERROR("invalid PIC mask\n");

  TEST_LEAVE();
}
