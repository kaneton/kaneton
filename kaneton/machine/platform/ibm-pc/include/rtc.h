/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...on/machine/platform/ibm-pc/include/rtc.h
 *
 * created       julien quintard   [wed nov 24 12:48:27 2010]
 * updated       julien quintard   [thu dec  9 15:22:26 2010]
 */

#ifndef PLATFORM_RTC_H
#define PLATFORM_RTC_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * these macros provides the address, registers, options and flags
 * related to the RTC.
 */

#define PLATFORM_RTC_ADDRESS			0x70
#define PLATFORM_RTC_DATA			0x71

#define PLATFORM_RTC_REGISTER_SECOND		0x00
#define PLATFORM_RTC_REGISTER_MINUTE		0x02
#define PLATFORM_RTC_REGISTER_HOUR		0x04
#define PLATFORM_RTC_REGISTER_WEEKDAY		0x06
#define PLATFORM_RTC_REGISTER_DAY_OF_MONTH	0x07
#define PLATFORM_RTC_REGISTER_MONTH		0x08
#define PLATFORM_RTC_REGISTER_YEAR		0x09
#define PLATFORM_RTC_REGISTER_STATUS_A		0x0a
#define PLATFORM_RTC_REGISTER_STATUS_B		0x0b
#define PLATFORM_RTC_REGISTER_CENTURY		0x32

#define PLATFORM_RTC_FORMAT_24H			0x02
#define PLATFORM_RTC_FORMAT_BCD			0x04

#define PLATFORM_RTC_OPTION_DST			0x01 /* daylight saving time */

#define PLATFORM_RTC_FORMAT			PLATFORM_RTC_FORMAT_BCD

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this structure represents the current state of the RTC.
 */

typedef struct
{
  t_uint32		millisecond;

  t_uint8		second;
  t_uint8		minute;
  t_uint8		hour;

  t_uint8		day;
  t_uint8		month;
  t_uint8		year;

  t_uint8		weekday;
  t_uint8		century;
}			ps_rtc_state;

/*
 * the RTC manager.
 */

typedef struct
{
  ps_rtc_state		state;
}			pm_rtc;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../rtc.c
 *
 */

/*
 * ../rtc.c
 */

void			platform_rtc_dump(ps_rtc_state*		rtc);

t_status		platform_rtc_write(t_uint8		address,
					   t_uint8		value);

t_status		platform_rtc_read(t_uint8		address,
					  t_uint8*		value);

t_status		platform_rtc_extract(t_uint8		address,
					     t_uint8*		value);

t_status		platform_rtc_load(ps_rtc_state*		rtc);

t_status		platform_rtc_state(ps_rtc_state**	rtc);

t_status		platform_rtc_update(t_uint32		millisecond);

t_status		platform_rtc_initialize(void);

t_status		platform_rtc_clean(void);


/*
 * eop
 */

#endif
