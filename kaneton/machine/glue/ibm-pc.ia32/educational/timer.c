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
 * updated       julien quintard   [sun jan 30 20:42:39 2011]
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
 *
 * steps:
 *
 * 1) initialize the PIT - Programmable Interrupt Timer.
 * 2) reserve the timer event in order to trigger a function on a regular
 *    basis.
 */

t_status		glue_timer_initialize(void)
{
  /*
   * 1)
   */

  if (platform_pit_initialize() != STATUS_OK)
    MACHINE_ESCAPE("unable to initialize the PIT");

  /*
   * 2)
   */

  if (event_reserve(ARCHITECTURE_IDT_IRQ_PIT,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(timer_handler),
		    0) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve the event corresponding to the hardware "
		   "timer interrupt");

  MACHINE_LEAVE();
}

/*
 * this function cleans the timer manager's glue.
 *
 * steps:
 *
 * 1) release the timer event.
 * 2) clean the PIT.
 */

t_status		glue_timer_clean(void)
{
  /*
   * 1)
   */

  if (event_release(ARCHITECTURE_IDT_IRQ_PIT) != STATUS_OK)
    MACHINE_ESCAPE("unable to release the timer IRQ");

  /*
   * 2)
   */

  if (platform_pit_clean() != STATUS_OK)
    MACHINE_ESCAPE("unable to clean the PIT");

  MACHINE_LEAVE();
}
