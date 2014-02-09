/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...cture/ia32/educational/event/pic/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat feb  5 21:14:19 2011]
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

void			test_architecture_event_pic_01_handler(t_id	id)
{
}

void			test_architecture_event_pic_01(void)
{
  t_uint8		mask;

  TEST_ENTER();

  if (event_reserve(ARCHITECTURE_IDT_IRQ_FLOPPY,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_pic_01),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  ARCHITECTURE_INB(TEST_MASTER_PORT_B, mask);

  if ((mask & (1 << 6)) != 0)
    TEST_ERROR("invalid PIC mask");

  ARCHITECTURE_INB(TEST_SLAVE_PORT_B, mask);

  if (mask != 0xff)
    TEST_ERROR("invalid PIC mask");

  TEST_SIGNATURE(zxcmewoiveroiger);

  TEST_LEAVE();
}
