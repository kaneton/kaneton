/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license
 *
 * file          /home/buckman/cry...e/glue/ibm-pc.ia32/educational/message.c
 *
 * created       matthieu bucchianeri   [sun jun 17 18:16:18 2007]
 * updated       matthieu bucchianeri   [wed jan  9 11:51:43 2008]
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

#include <libc/libc.h>
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
    /* [cut k4] */
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
    /* [/cut] */
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/* [cut k4] */
/*
 * set return code for a blocked syscall.
 */

t_error			glue_message_return(i_thread		thread,
					    t_error		code)
{
  MESSAGE_ENTER(message);

  if (ia32_syscall_set_code(thread, code) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * set return info for a blocked syscall.
 */

t_error			glue_message_return_info(i_thread	thread,
						 t_error	code,
						 t_vsize	size,
						 i_node		sender)
{
  MESSAGE_ENTER(message);

  if (ia32_syscall_set_info(thread, code, size, sender) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * initialize machdep part of message manager.
 */

t_error		glue_message_initialize(void)
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
/* [/cut] */
