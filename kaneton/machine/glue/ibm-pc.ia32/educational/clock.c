/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ine/glue/ibm-pc.ia32/educational/clock.c
 *
 * created       julien quintard   [wed nov 24 19:11:47 2010]
 * updated       julien quintard   [fri dec 10 16:16:48 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the clock managment on a ibm-pc.ia32
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
 * the ibm-pc.ia32 clock manager dispatch.
 */

d_clock			glue_clock_dispatch =
  {
    NULL,
    glue_clock_update,
    glue_clock_current,
    glue_clock_initialize,
    glue_clock_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function updates the clock with the given number of milliseconds.
 *
 * steps:
 *
 * 1) call the RTC update() function.
 */

t_error			glue_clock_update(t_uint32		millisecond)
{
  /*
   * 1)
   */

  if (platform_rtc_update(millisecond) != ERROR_OK)
    MACHINE_ESCAPE("unable to update the RTC");

  MACHINE_LEAVE();
}

/*
 * return the current date/time through the clock structure.
 *
 * steps:
 *
 * 1) call the RTC to retrieve the hardware information related to the date
 *    and time.
 * 2) fill the clock structure with the RTC.
 */

t_error			glue_clock_current(s_clock*		clock)
{
  ps_rtc_state*		rtc;

  /*
   * 1)
   */

  if (platform_rtc_state(&rtc) != ERROR_OK)
    MACHINE_ESCAPE("unable to load the RTC date/time structure");

  /*
   * 2)
   */

  clock->millisecond = rtc->millisecond;
  clock->second = rtc->second;
  clock->minute = rtc->minute;
  clock->hour = rtc->hour;

  clock->day = rtc->day;
  clock->month = rtc->month;
  clock->year = rtc->century * 100 + rtc->year;

  MACHINE_LEAVE();
}

/*
 * initialize the clock on the platform.
 *
 * steps:
 *
 * 1) initialize the RTC.
 */

t_error			glue_clock_initialize(void)
{
  /*
   * 1)
   */

  if (platform_rtc_initialize() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the real time clock");

  MACHINE_LEAVE();
}

/*
 * clean the clock.
 */

t_error			glue_clock_clean(void)
{
  MACHINE_LEAVE();
}
