/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license
 *
 * file          /home/mycure/kane...e/glue/ibm-pc.ia32/educational/message.c
 *
 * created       matthieu bucchianeri   [sun jun 17 18:16:18 2007]
 * updated       julien quintard   [mon nov 22 22:27:38 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependent code for message manager on ia32 with paging
 * architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_message*	_message;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 message manager dispatch.
 */

d_message		       	message_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_message_return,
    glue_message_return_info,
    glue_message_initialize,
    glue_message_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * set return code for a blocked syscall.
 */

t_error			glue_message_return(i_thread		thread,
					    t_error		code)
{
  MESSAGE_ENTER(_message);

  if (ia32_syscall_set_code(thread, code) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * set return info for a blocked syscall.
 */

t_error			glue_message_return_info(i_thread	thread,
						 t_error	code,
						 t_vsize	size,
						 i_node		sender)
{
  MESSAGE_ENTER(_message);

  if (ia32_syscall_set_info(thread, code, size, sender) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * initialize machine part of message manager.
 */

t_error		glue_message_initialize(void)
{
  MESSAGE_ENTER(_message);

  if (ia32_syscalls_init() != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * clean machine part of message manager.
 */

t_error		glue_message_clean(void)
{
  MESSAGE_ENTER(_message);

  if (ia32_syscalls_clean() != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}
