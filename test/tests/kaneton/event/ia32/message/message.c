/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/event/ia32/message/message.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [tue nov 16 20:20:28 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "message.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task		ktask;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_event_ia32_message(void)
{
  o_event_message	evt;
  t_vsize		sz;
  i_node		sender;

  TEST_ENTER();

  if (event_reserve(3,
		    EVENT_MESSAGE,
		    EVENT_TASK(ktask),
		    0x42424242) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("int $3");

  if (message_poll(ktask,
		   MESSAGE_TYPE_EVENT,
		   (t_vaddr)&evt,
		   &sz,
		   &sender) != ERROR_NONE)
    TEST_ERROR("[message_poll] error: this means the exception has not been caught\n");

  if (evt.id != 3)
    TEST_ERROR("invalid message's event identifier\n");

  if (evt.data != 0x42424242)
    TEST_ERROR("invalid message's event data\n");

  if (sender.task != 0)
    TEST_ERROR("invalid message's sender\n");

  TEST_LEAVE();
}
