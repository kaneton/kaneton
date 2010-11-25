/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ton/machine/glue/ibm-pc.ia32/time/time.h
 *
 * created       julien quintard   [thu nov 18 20:10:44 2010]
 * updated       julien quintard   [wed nov 24 13:13:20 2010]
 */

#ifndef TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_TIME_TIME_H
#define TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_TIME_TIME_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../engine/engine.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define TEST_RTC_STATUSA	0x0a	/* status register A */
#define TEST_RTCSA_TUP		0x80	/* time update, don't look now */
#define TEST_RTCSA_DIVIDER	0x20	/* divider correct for 32768 Hz */
#define TEST_RTCSA_8192		0x03
#define TEST_RTCSA_4096		0x04
#define TEST_RTCSA_2048		0x05
#define TEST_RTCSA_1024		0x06
#define TEST_RTCSA_512		0x07
#define TEST_RTCSA_256		0x08
#define TEST_RTCSA_128		0x09
#define TEST_RTCSA_64		0x0a
#define TEST_RTCSA_32		0x0b

#define TEST_RTC_STATUSB	0x0b	/* status register B */
#define TEST_RTCSB_DST		0x01	/* Daylight Savings Time enable */
#define TEST_RTCSB_24HR		0x02	/* 0 = 12 hours, 1 = 24 hours */
#define TEST_RTCSB_BCD		0x04	/* 0 = BCD, 1 = Binary coded time */
#define TEST_RTCSB_SQWE		0x08	/* 1 = output sqare wave at SQW pin */
#define TEST_RTCSB_UINTR	0x10	/* 1 = enable update-ended interrupt */
#define TEST_RTCSB_AINTR	0x20	/* 1 = enable alarm interrupt */
#define TEST_RTCSB_PINTR	0x40	/* 1 = enable periodic clock interrupt */
#define TEST_RTCSB_HALT		0x80	/* stop clock updates */

#define TEST_RTC_INTR		0x0c	/* status register C (R) interrupt source */
#define TEST_RTCIR_UPDATE	0x10	/* update intr */
#define TEST_RTCIR_ALARM	0x20	/* alarm intr */
#define TEST_RTCIR_PERIOD	0x40	/* periodic intr */
#define TEST_RTCIR_INT		0x80	/* interrupt output signal */

#define TEST_MASTER_PORT_A	0x20
#define TEST_MASTER_PORT_B	0x21
#define TEST_SLAVE_PORT_A	0xa0
#define TEST_SLAVE_PORT_B	0xa1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      time.c
 */

/*
 * time.c
 */

t_uint8			test_ibmpc_ia32_time_cmos(void);

t_uint64		test_ibmpc_ia32_time_rtc(void);

unsigned char		test_ibmpc_ia32_time_read(unsigned char	reg);

void			test_ibmpc_ia32_time_write(unsigned char	reg,
						    unsigned char	val);

void			test_ibmpc_ia32_time_rtc_handler(void);

void			test_ibmpc_ia32_time_rtc_setup(void);


/*
 * eop
 */

#endif
