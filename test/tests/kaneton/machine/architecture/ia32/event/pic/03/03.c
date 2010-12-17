/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/event/pic/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [thu dec 16 13:34:37 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"


/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_pic_03_handler(t_id	id)
{
}

void			test_architecture_event_pic_03(void)
{
  t_uint8		mask;

  TEST_ENTER();

  if (event_reserve(32 + 3,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_pic_03),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_release(32 + 3) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  INB(TEST_MASTER_PORT_B, mask);

  if ((mask & (1 << 3)) == 0)
    TEST_ERROR("invalid PIC mask");

  INB(TEST_SLAVE_PORT_B, mask);

  if (mask != 0xff)
    TEST_ERROR("invalid PIC mask");

  TEST_SIGNATURE(mzsdqwdwfjwgomre);

  TEST_LEAVE();
}
