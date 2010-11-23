/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ine/glue/ibm-pc.ia32/educational/timer.c
 *
 * created       julien quintard   [mon jun 11 05:41:14 2007]
 * updated       julien quintard   [mon nov 22 22:31:56 2010]
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

extern m_timer*		_timer;

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
 *
 */

t_error			glue_timer_initialize(void)
{
  TIMER_ENTER(_timer);

  if (ibmpc_timer_init() != ERROR_OK)
    TIMER_LEAVE(_timer, ERROR_KO);

  if (event_reserve(32, EVENT_FUNCTION, EVENT_HANDLER(timer_handler),
		    0) != ERROR_OK)
    TIMER_LEAVE(_timer, ERROR_KO);

  TIMER_LEAVE(_timer, ERROR_OK);
}

