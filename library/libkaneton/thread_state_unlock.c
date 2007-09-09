/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...library/libkaneton/thread_state_unlock.c
 *
 * created       matthieu bucchianeri   [sat aug 25 00:25:17 2007]
 * updated       matthieu bucchianeri   [mon sep 10 00:11:39 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements special lock management for thread_state_unlock.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <stdint.h>
#include <core/core.h>
#include <libkaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

t_error		thread_state_unlock(t_id		arg1,
				    t_state		arg2,
				    t_uint8*		lock)
{
  o_syscall		message;
  i_node		node;
  t_vsize		size;

  node.machine = 0;
  node.task = 0;

  message.id = 0;

  message.u.request.operation = INTERFACE_THREAD_STATE;
  message.u.request.u.thread_state.arg1 = arg1;
  message.u.request.u.thread_state.arg2 = arg2;

  message_send_unlock(node, MESSAGE_TYPE_INTERFACE, (t_vaddr)&message, sizeof (message), lock);

  if (message_receive(MESSAGE_TYPE_INTERFACE, (t_vaddr)&message, &size, &node) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (message.u.reply.error != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}
