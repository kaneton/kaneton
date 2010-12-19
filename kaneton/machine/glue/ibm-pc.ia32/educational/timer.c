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
 * updated       julien quintard   [sun dec 19 18:30:32 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the timer manager's glue.
 *
 * the machine relies on the platform which provides a PIT - Programmable
 * Interval Timer.
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
 * the timer dispatcher.
 */

d_timer			glue_timer_dispatch =
  {
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
 * this function initializes the timer manager's glue.
 */

t_error			glue_timer_initialize(void)
{
  /*
   * XXX
   */

  if (platform_pit_initialize() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the PIT");

  /*
   * XXX
   */

  if (event_reserve(32,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(timer_handler),
		    0) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the event corresponding to the hardware "
		   "timer interrupt");

  MACHINE_LEAVE();
}

/*
 * this function cleans the timer manager's glue.
 */

t_error			glue_timer_clean(void)
{
  /*
   * XXX
   */

  if (platform_pit_clean() != ERROR_OK)
    MACHINE_ESCAPE("unable to clean the PIT");

  MACHINE_LEAVE();
}
