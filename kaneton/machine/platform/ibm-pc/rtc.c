/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/machine/platform/ibm-pc/rtc.c
 *
 * created       julien quintard   [wed nov 24 10:17:08 2010]
 * updated       julien quintard   [sun jan 30 13:01:05 2011]
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

pm_rtc			_platform_rtc;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the RTC structure's attributes.
 */

void			platform_rtc_dump(ps_rtc_state*		rtc)
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

t_status		platform_rtc_write(t_uint8		address,
					   t_uint8		value)
{
  ARCHITECTURE_IO_OUT_8(PLATFORM_RTC_ADDRESS, address);
  ARCHITECTURE_IO_OUT_8(PLATFORM_RTC_DATA, value);

  MACHINE_LEAVE();
}

/*
 * this function fetches the value of the given RTC register.
 */

t_status		platform_rtc_read(t_uint8		address,
					  t_uint8*		value)
{
  ARCHITECTURE_IO_OUT_8(PLATFORM_RTC_ADDRESS, address);
  ARCHITECTURE_IO_IN_8(PLATFORM_RTC_DATA, *value);

  MACHINE_LEAVE();
}

/*
 * this function reads a RTC register and converts it into
 * binary so that the caller can use it.
 */

t_status		platform_rtc_extract(t_uint8		address,
					     t_uint8*		value)
{
  switch (PLATFORM_RTC_FORMAT)
    {
    case PLATFORM_RTC_FORMAT_BCD:
      {
	t_uint8		bcd;

	if (platform_rtc_read(address, &bcd) != STATUS_OK)
	  MACHINE_ESCAPE("unable to read the RTC");

	*value = (bcd >> 1) + (bcd >> 3) + (bcd & 0xf);

	break;
      }
    case PLATFORM_RTC_FORMAT_24H:
    default:
      {
	MACHINE_ESCAPE("unsupported format '%u'",
		       PLATFORM_RTC_FORMAT);
      }
    }

  MACHINE_LEAVE();
}

/*
 * this function reads all the registers composing the RTC
 * structure.
 *
 * note however that since the RTC does not handle milliseconds, the
 * milliseconds attribute is initially set to zero.
 */

t_status		platform_rtc_load(ps_rtc_state*		rtc)
{
  rtc->millisecond = 0;

  if (platform_rtc_extract(PLATFORM_RTC_REGISTER_SECOND,
			   &rtc->second) != STATUS_OK)
    MACHINE_ESCAPE("unable to extract the RTC's seconds");

  if (platform_rtc_extract(PLATFORM_RTC_REGISTER_MINUTE,
			   &rtc->minute) != STATUS_OK)
    MACHINE_ESCAPE("unable to extract the RTC's minutes");

  if (platform_rtc_extract(PLATFORM_RTC_REGISTER_HOUR,
			   &rtc->hour) != STATUS_OK)
    MACHINE_ESCAPE("unable to extract the RTC's hours");

  if (platform_rtc_extract(PLATFORM_RTC_REGISTER_DAY_OF_MONTH,
			   &rtc->day) != STATUS_OK)
    MACHINE_ESCAPE("unable to extract the RTC's days");

  if (platform_rtc_extract(PLATFORM_RTC_REGISTER_MONTH,
			   &rtc->month) != STATUS_OK)
    MACHINE_ESCAPE("unable to extract the RTC's months");

  if (platform_rtc_extract(PLATFORM_RTC_REGISTER_YEAR,
			   &rtc->year) != STATUS_OK)
    MACHINE_ESCAPE("unable to extract the RTC's years");

  if (platform_rtc_extract(PLATFORM_RTC_REGISTER_WEEKDAY,
			   &rtc->weekday) != STATUS_OK)
    MACHINE_ESCAPE("unable to extract the RTC's weekday");

  if (platform_rtc_extract(PLATFORM_RTC_REGISTER_CENTURY,
			   &rtc->century) != STATUS_OK)
    MACHINE_ESCAPE("unable to extract the RTC's century");

  MACHINE_LEAVE();
}

/*
 * this function returns a pointer on the current RTC state structure.
 */

t_status		platform_rtc_state(ps_rtc_state**	rtc)
{
  *rtc = &_platform_rtc.state;

  MACHINE_LEAVE();
}

/*
 * this function increases the number of milliseconds and possibly updates the
 * RTC structure.
 *
 * steps:
 *
 * 1) increase the number of milliseconds.
 * 2) adjust the attributes since the number of seconds
 *    can exceed 60 for instance, and so on for the other
 *    attributes.
 */

t_status		platform_rtc_update(t_uint32		millisecond)
{
  /*
   * 1)
   */

  _platform_rtc.state.millisecond += millisecond;

  /*
   * 2)
   */

  if (_platform_rtc.state.millisecond > 1000)
    {
      _platform_rtc.state.second += _platform_rtc.state.millisecond / 1000;
      _platform_rtc.state.millisecond = _platform_rtc.state.millisecond % 1000;
    }

  if (_platform_rtc.state.second > 60)
    {
      _platform_rtc.state.minute += _platform_rtc.state.second / 60;
      _platform_rtc.state.second = _platform_rtc.state.second % 60;
    }

  if (_platform_rtc.state.minute > 60)
    {
      _platform_rtc.state.hour += _platform_rtc.state.minute / 60;
      _platform_rtc.state.minute = _platform_rtc.state.minute % 60;
    }

  if (_platform_rtc.state.hour > 24)
    {
      _platform_rtc.state.day += _platform_rtc.state.hour / 24;
      _platform_rtc.state.hour = _platform_rtc.state.hour % 24;
    }

  MACHINE_LEAVE();
}

/*
 * this function initializes the RTC with several options.
 *
 * steps:
 *
 * 1) initialize the manager's structure.
 * 2) activate the BCD format.
 * 3) load the initial RTC.
 */

t_status		platform_rtc_initialize(void)
{
  t_uint8		status;

  /*
   * 1)
   */

  memset(&_platform_rtc, 0x0, sizeof (pm_rtc));

  /*
   * 2)
   */

  if (platform_rtc_read(PLATFORM_RTC_REGISTER_STATUS_B, &status) != STATUS_OK)
    MACHINE_ESCAPE("unable to read the RTC");

  if (platform_rtc_write(PLATFORM_RTC_REGISTER_STATUS_B,
			 status |
			 PLATFORM_RTC_FORMAT_BCD) != STATUS_OK)
    MACHINE_ESCAPE("unable to write the RTC");

  /*
   * 2)
   */

  if (platform_rtc_load(&_platform_rtc.state) != STATUS_OK)
    MACHINE_ESCAPE("unable to load the initial RTC");

  MACHINE_LEAVE();
}

/*
 * this function cleans the RTC.
 */

t_status		platform_rtc_clean(void)
{
  MACHINE_LEAVE();
}
