/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...kaneton/machine/glue/ibm-pc.ia32/timer.c
 *
 * created       julien quintard   [mon jun 11 05:41:14 2007]
 * updated       matthieu bucchianeri   [mon jul 30 16:51:39 2007]
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

#include <libc/libc.h>
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
/*								   [cut] k2 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_timer_initialize,
    NULL,
/*								   [cut] /k2 */
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*								   [cut] k2 */

/*
 * init the timer on the ibm-pc.ia32 machine.
 */

t_error			glue_timer_initialize(void)
{
  TIMER_ENTER(timer);

  if (ibmpc_timer_init() != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  if (event_reserve(32, EVENT_FUNCTION, EVENT_HANDLER(timer_handler),
		    0) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*								   [cut] /k2 */
