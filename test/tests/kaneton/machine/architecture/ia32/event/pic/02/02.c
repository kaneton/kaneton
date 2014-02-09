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
 * updated       julien quintard   [sat jan 15 16:37:33 2011]
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
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_pic_02),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  ARCHITECTURE_INB(TEST_MASTER_PORT_B, mask);

  if ((mask & (1 << 2)) != 0)
    TEST_ERROR("invalid PIC mask");

  ARCHITECTURE_INB(TEST_SLAVE_PORT_B, mask);

  if (mask != 0xfd)
    TEST_ERROR("invalid PIC mask");

  TEST_SIGNATURE(vniwojw0f32wg3hh);

  TEST_LEAVE();
}
