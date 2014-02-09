/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chine/glue/ibm-pc.ia32/optimised/timer.c
 *
 * created       julien quintard   [mon jun 11 05:41:14 2007]
 * updated       julien quintard   [fri may  1 19:44:30 2009]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the timer managment on a ibm-pc.ia32
 * machine.
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

extern m_timer*		timer;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 timer manager dispatch.
 */

d_timer				timer_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_timer_initialize,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */


/*
 * init the timer on the ibm-pc.ia32 machine.
 */

t_status		glue_timer_initialize(void)
{
  TIMER_ENTER(timer);

  if (ibmpc_timer_init() != STATUS_OK)
    TIMER_LEAVE(timer, STATUS_UNKNOWN_ERROR);

  if (event_reserve(32, EVENT_FUNCTION, EVENT_HANDLER(timer_handler),
		    0) != STATUS_OK)
    TIMER_LEAVE(timer, STATUS_UNKNOWN_ERROR);

  TIMER_LEAVE(timer, STATUS_OK);
}

