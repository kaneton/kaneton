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
 * updated       julien quintard   [thu jan  6 09:06:57 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the message manager's glue.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the message dispatcher.
 */

d_message		glue_message_dispatch =
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
 *
 * XXX
 */

t_error			glue_message_return(i_thread		thread,
					    t_error		code)
{
  /* XXX
  if (ia32_syscall_set_code(thread, code) != ERROR_OK)
    MACHINE_ESCAPE("XXX");
  */

  MACHINE_LEAVE();
}

/*
 * set return info for a blocked syscall.
 *
 * XXX
 */

t_error			glue_message_return_info(i_thread	thread,
						 t_error	code,
						 t_vsize	size,
						 i_node		sender)
{
  /* XXX
  if (ia32_syscall_set_info(thread, code, size, sender) != ERROR_OK)
    MACHINE_ESCAPE("XXX");
  */

  MACHINE_LEAVE();
}

/*
 * initialize machine part of message manager.
 *
 * XXX
 */

t_error			glue_message_initialize(void)
{
  /* XXX
  if (ia32_syscalls_init() != ERROR_OK)
    MACHINE_ESCAPE("XXX");
  */

  MACHINE_LEAVE();
}

/*
 * clean machine part of message manager.
 *
 * XXX
 */

t_error			glue_message_clean(void)
{
  /* XXX
  if (ia32_syscalls_clean() != ERROR_OK)
    MACHINE_ESCAPE("XXX");
  */

  MACHINE_LEAVE();
}
