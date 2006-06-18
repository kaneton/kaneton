/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/message/message.c
 *
 * created       matthieu bucchianeri   [sat jun 17 19:37:05 2006]
 * updated       matthieu bucchianeri   [sat jun 17 19:50:53 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(message);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * message manager variable.
 */

m_message*		message = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                 [cut] /k6 */

/*
 * this function initialises the message manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) initialise a statistic object.
 * 3) call the machine dependent code.
 */

t_error			message_init(void)
{
  /*
   * 1)
   */

  if ((message = malloc(sizeof(m_message))) == NULL)
    {
      cons_msg('!', "message: cannot allocate memory for the message "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(message, 0x0, sizeof(m_message));

  /*
   * 2)
   */

  STATS_RESERVE("message", &message->stats);

  /*
   * 3)
   */

  if (machdep_call(message, message_init) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);

}

/*
 * this function cleans the message manager.
 *
 * steps:
 *
 * 1) call the dependent code.
 * 2) release the statistics object.
 * 3) free the manager structure.
 */

t_error			message_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(message, message_clean) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  /*
   * 2)
   */

  STATS_RELEASE(message->stats);

  /*
   * 3)
   */

  free(message);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k6 */
