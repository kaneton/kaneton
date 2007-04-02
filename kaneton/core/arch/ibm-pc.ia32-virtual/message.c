/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/message.c
 *
 * created       matthieu bucchianeri   [sat jun 17 19:38:22 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:41:19 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_message*	message;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the message manager dispatch.
 */

d_message		message_dispatch =
  {

/*                                                                  [cut] k4 */

    ia32_message_init,
    ia32_message_clean

/*                                                                 [cut] /k4 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k4 */

void			ia32_message_send_handler(void)
{
  printf("send\n");
}

void			ia32_message_recv_handler(void)
{
  printf("recv\n");
}

t_error			ia32_message_init(void)
{
  if (event_reserve(56, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_message_send_handler)) != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (event_reserve(57, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_message_recv_handler)) != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_NONE;
}

t_error			ia32_message_clean(void)
{
  if (event_release(56) != ERROR_NONE ||
      event_release(57) != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*                                                                 [cut] /k4 */
