/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/event/message/message.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat dec  4 12:21:45 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "message.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_message(void)
{
  o_event_message	evt;
  t_vsize		sz;
  i_node		sender;

  TEST_ENTER();

  if (event_reserve(3,
		    EVENT_MESSAGE,
		    EVENT_TASK(_kernel->task),
		    0x42424242) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  asm volatile("int $3");

  if (message_poll(_kernel->task,
		   MESSAGE_TYPE_EVENT,
		   (t_vaddr)&evt,
		   &sz,
		   &sender) != ERROR_OK)
    TEST_ERROR("[message_poll] error: this means the exception "
	       "has not been caught");

  if (evt.id != 3)
    TEST_ERROR("invalid message's event identifier");

  if (evt.data != 0x42424242)
    TEST_ERROR("invalid message's event data");

  if (sender.task != 0)
    TEST_ERROR("invalid message's sender");

  TEST_SIGNATURE(98sdcujwefqwndff230);

  TEST_LEAVE();
}
