/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/event/ia32/clock/clock.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [tue nov 16 19:15:09 2010]
 */

#ifndef TESTS_KANETON_EVENT_IA32_CLOCK_CLOCK_H
#define TESTS_KANETON_EVENT_IA32_CLOCK_CLOCK_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../engine/engine.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define RTC_STATUSA	0x0a	/* status register A */
#define RTCSA_TUP	0x80	/* time update, don't look now */
#define RTCSA_DIVIDER	0x20	/* divider correct for 32768 Hz */
#define RTCSA_8192	0x03
#define RTCSA_4096	0x04
#define RTCSA_2048	0x05
#define RTCSA_1024	0x06
#define RTCSA_512	0x07
#define RTCSA_256	0x08
#define RTCSA_128	0x09
#define RTCSA_64	0x0a
#define RTCSA_32	0x0b

#define RTC_STATUSB	0x0b	/* status register B */
#define RTCSB_DST	0x01	/* Daylight Savings Time enable */
#define RTCSB_24HR	0x02	/* 0 = 12 hours, 1 = 24 hours */
#define RTCSB_BCD	0x04	/* 0 = BCD, 1 = Binary coded time */
#define RTCSB_SQWE	0x08	/* 1 = output sqare wave at SQW pin */
#define RTCSB_UINTR	0x10	/* 1 = enable update-ended interrupt */
#define RTCSB_AINTR	0x20	/* 1 = enable alarm interrupt */
#define RTCSB_PINTR	0x40	/* 1 = enable periodic clock interrupt */
#define RTCSB_HALT	0x80	/* stop clock updates */

#define RTC_INTR	0x0c	/* status register C (R) interrupt source */
#define RTCIR_UPDATE	0x10	/* update intr */
#define RTCIR_ALARM	0x20	/* alarm intr */
#define RTCIR_PERIOD	0x40	/* periodic intr */
#define RTCIR_INT	0x80	/* interrupt output signal */

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      clock.c
 */

/*
 * clock.c
 */

t_uint8			test_event_ia32_clock_read(unsigned char	reg);

void			test_event_ia32_clock_write(unsigned char	reg,
						    unsigned char	val);

void			test_event_ia32_clock_handler(t_id		id);

void			test_event_ia32_clock_content(void);

void			test_event_ia32_clock(void);


/*
 * eop
 */

#endif
