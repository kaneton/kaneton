/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...hine/architecture/ia32/event/pic/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [wed dec  1 05:50:32 2010]
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

void			test_architecture_event_pic_02_handler(t_id		id)
{
}

void			test_architecture_event_pic_02(void)
{
  t_uint8		mask;

  TEST_ENTER();

  if (event_reserve(32 + 9,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_architecture_event_pic_02),
		    0) != ERROR_OK)
    TEST_ERROR("[event_reserve] error\n");

  INB(TEST_MASTER_PORT_B, mask);

  if ((mask & (1 << 2)) != 0)
    TEST_ERROR("invalid PIC mask\n");

  INB(TEST_SLAVE_PORT_B, mask);

  if (mask != 0xfd)
    TEST_ERROR("invalid PIC mask\n");

  TEST_SIGNATURE(vniwojw0f32wg3hh);

  TEST_LEAVE();
}
