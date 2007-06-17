/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license
 *
 * file          /home/buckman/kan...neton/machine/glue/ibm-pc.ia32/message.c
 *
 * created       matthieu bucchianeri   [sun jun 17 18:16:18 2007]
 * updated       matthieu bucchianeri   [sun jun 17 18:31:18 2007]
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

#include <libc.h>
#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_message*	message;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 message manager dispatch.
 */

d_message		       	message_dispatch =
  {
    glue_message_init,
    glue_message_clean,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * initialize machdep part of message manager.
 */

t_error		glue_message_init(void)
{
  MESSAGE_ENTER(message);

  if (ia32_syscalls_init() != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * clean machdep part of message manager.
 */

t_error		glue_message_clean(void)
{
  MESSAGE_ENTER(message);

  if (ia32_syscalls_clean() != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}
