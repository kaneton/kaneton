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
 * updated       julien quintard   [sat jan 15 06:10:00 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the clock manager's glue.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the clock dispatcher.
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

t_status		glue_clock_update(t_uint32		millisecond)
{
  /*
   * 1)
   */

  if (platform_rtc_update(millisecond) != STATUS_OK)
    MACHINE_ESCAPE("unable to update the RTC");

  MACHINE_LEAVE();
}

/*
 * this function returns the current date/time through the clock structure.
 *
 * steps:
 *
 * 1) call the RTC to retrieve the hardware information related to the date
 *    and time.
 * 2) fill the clock structure with the RTC.
 */

t_status		glue_clock_current(s_clock*		clock)
{
  ps_rtc_state*		rtc;

  /*
   * 1)
   */

  if (platform_rtc_state(&rtc) != STATUS_OK)
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
 * this function initializes the clock manager by setting up the
 * platform's RTC.
 *
 * steps:
 *
 * 1) initialize the RTC.
 */

t_status		glue_clock_initialize(void)
{
  /*
   * 1)
   */

  if (platform_rtc_initialize() != STATUS_OK)
    MACHINE_ESCAPE("unable to initialize the real time clock");

  MACHINE_LEAVE();
}

/*
 * this function cleans the clock.
 *
 * 1) clean the RTC
 */

t_status		glue_clock_clean(void)
{
  /*
   * 1)
   */

  if (platform_rtc_clean() != STATUS_OK)
    MACHINE_ESCAPE("unable to clean the real time clock");

  MACHINE_LEAVE();
}
