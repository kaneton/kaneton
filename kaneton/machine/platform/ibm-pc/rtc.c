/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...LE/kaneton/machine/platform/ibm-pc/rtc.c
 *
 * created       julien quintard   [wed nov 24 10:17:08 2010]
 * updated       julien quintard   [sat dec  4 22:56:43 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file provides functionalities related to the RTC - Real-Time Clock.
 *
 * although there are many ways to manage the date and time, the solution
 * chosen here consists in: (i) initially, read the date and time from the
 * CMOS/RTC, (ii) rely on the PIT's interrupt---which occur every
 * TIMER_DELAY---in order to keep track of time.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the RTC variable.
 */

m_platform_rtc		_platform_rtc;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the RTC structure's attributes.
 */

void			platform_rtc_dump(t_platform_rtc*	rtc)
{
  module_call(console, message,
	      '#', "dumping the RTC structure:\n");

  module_call(console, message,
	      '#', "  millisecond: %u\n",
	      rtc->millisecond);

  module_call(console, message,
	      '#', "  second: %u\n",
	      rtc->second);

  module_call(console, message,
	      '#', "  minute: %u\n",
	      rtc->minute);

  module_call(console, message,
	      '#', "  hour: %u\n",
	      rtc->hour);

  module_call(console, message,
	      '#', "  day: %u\n",
	      rtc->day);

  module_call(console, message,
	      '#', "  month: %u\n",
	      rtc->month);

  module_call(console, message,
	      '#', "  year: %u\n",
	      rtc->year);

  module_call(console, message,
	      '#', "  week day: %u\n",
	      rtc->weekday);

  module_call(console, message,
	      '#', "  century: %u\n",
	      rtc->century);
}

/*
 * this function updates a register of the RTC.
 */

void			platform_rtc_write(t_uint8		address,
					   t_uint8		value)
{
  ARCHITECTURE_IO_OUT_8(PLATFORM_RTC_ADDRESS, address);
  ARCHITECTURE_IO_OUT_8(PLATFORM_RTC_DATA, value);
}

/*
 * this function fetches the value of the given RTC register.
 */

t_uint8			platform_rtc_read(t_uint8		address)
{
  t_uint8		value;

  ARCHITECTURE_IO_OUT_8(PLATFORM_RTC_ADDRESS, address);
  ARCHITECTURE_IO_IN_8(PLATFORM_RTC_DATA, value);

  return (value);
}

/*
 * this function reads a RTC register and converts it into
 * binary so that the caller can use it.
 */

t_error			platform_rtc_extract(t_uint8		address,
					     t_uint8*		value)
{
  switch (PLATFORM_RTC_FORMAT)
    {
    case PLATFORM_RTC_FORMAT_BCD:
      {
	t_uint8		bcd;

	bcd = platform_rtc_read(address);

	*value = (bcd >> 1) + (bcd >> 3) + (bcd & 0xf);

	break;
      }
    case PLATFORM_RTC_FORMAT_24H:
    default:
      {
	MACHINE_ESCAPE("the 24-hour RTC format is not supported");
      }
    }

  MACHINE_LEAVE();
}

/*
 * this function reads all the registers composing the RTC
 * structure.
 */

t_error			platform_rtc_load(t_platform_rtc*	rtc)
{
  rtc->millisecond = 0;

  platform_rtc_extract(PLATFORM_RTC_REGISTER_SECOND,
		       &rtc->second);

  platform_rtc_extract(PLATFORM_RTC_REGISTER_MINUTE,
		       &rtc->minute);

  platform_rtc_extract(PLATFORM_RTC_REGISTER_HOUR,
		       &rtc->hour);

  platform_rtc_extract(PLATFORM_RTC_REGISTER_DAY_OF_MONTH,
		       &rtc->day);

  platform_rtc_extract(PLATFORM_RTC_REGISTER_MONTH,
		       &rtc->month);

  platform_rtc_extract(PLATFORM_RTC_REGISTER_YEAR,
		       &rtc->year);

  platform_rtc_extract(PLATFORM_RTC_REGISTER_WEEKDAY,
		       &rtc->weekday);

  platform_rtc_extract(PLATFORM_RTC_REGISTER_CENTURY,
		       &rtc->century);

  MACHINE_LEAVE();
}

/*
 * this function returns the current RTC structure.
 *
 * steps:
 *
 * 1) return the pointer on the internal RTC structure.
 */

t_error			platform_rtc_current(t_platform_rtc**	rtc)
{
  /*
   * 1)
   */

  *rtc = &_platform_rtc.rtc;

  MACHINE_LEAVE();
}

/*
 * this function increases the number of milliseconds and possibly updates the
 * RTC structure.
 *
 * steps:
 *
 * 1) increase the number of ticks.
 * 2) adjust the attributes since the number of seconds
 *    can exceed 60 for instance, and so on for the other
 *    values.
 */

t_error			platform_rtc_update(t_uint32		millisecond)
{
  /*
   * 1)
   */

  _platform_rtc.rtc.millisecond += millisecond;

  /*
   * 2)
   */

  if (_platform_rtc.rtc.millisecond > 1000)
    {
      _platform_rtc.rtc.second += _platform_rtc.rtc.millisecond / 1000;
      _platform_rtc.rtc.millisecond = _platform_rtc.rtc.millisecond % 1000;
    }

  if (_platform_rtc.rtc.second > 60)
    {
      _platform_rtc.rtc.minute += _platform_rtc.rtc.second / 60;
      _platform_rtc.rtc.second = _platform_rtc.rtc.second % 60;
    }

  if (_platform_rtc.rtc.minute > 60)
    {
      _platform_rtc.rtc.hour += _platform_rtc.rtc.minute / 60;
      _platform_rtc.rtc.minute = _platform_rtc.rtc.minute % 60;
    }

  if (_platform_rtc.rtc.hour > 24)
    {
      _platform_rtc.rtc.day += _platform_rtc.rtc.hour / 24;
      _platform_rtc.rtc.hour = _platform_rtc.rtc.hour % 24;
    }

  MACHINE_LEAVE();
}

/*
 * this function initializes the RTC with several
 * options.
 *
 * steps:
 *
 * 1) activate the BCD format.
 * 2) load the initial RTC.
 */

t_error			platform_rtc_initialize(void)
{
  /*
   * 1)
   */

  platform_rtc_write(PLATFORM_RTC_REGISTER_STATUS_B,
		     platform_rtc_read(PLATFORM_RTC_REGISTER_STATUS_B) |
		     PLATFORM_RTC_FORMAT_BCD);

  /*
   * 2)
   */

  if (platform_rtc_load(&_platform_rtc.rtc) != ERROR_OK)
    MACHINE_ESCAPE("unable to load the initial RTC");

  MACHINE_LEAVE();
}

/*
 * this function cleans the RTC.
 */

t_error			platform_rtc_clean(void)
{
  MACHINE_LEAVE();
}
