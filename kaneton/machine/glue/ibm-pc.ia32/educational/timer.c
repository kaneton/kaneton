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
 * updated       julien quintard   [fri dec 10 10:31:17 2010]
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
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 timer manager dispatch.
 */

d_timer			glue_timer_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_timer_initialize,
    glue_timer_clean,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * init the timer on the platform.
 *
 */

t_error			glue_timer_initialize(void)
{
  if (platform_pit_initialize() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the PIT");

  if (event_reserve(32,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(timer_handler),
		    0) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the event corresponding to the hardware "
		   "timer interrupt");

  MACHINE_LEAVE();
}

/*
 * clean the timer.
 */

t_error			glue_timer_clean(void)
{
  if (platform_pit_clean() != ERROR_OK)
    MACHINE_ESCAPE("unable to clean the PIT");

  MACHINE_LEAVE();
}
