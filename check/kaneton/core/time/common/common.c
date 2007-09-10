/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [sat oct 14 16:56:32 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "common.h"

void	check_time_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to time manager
 */

void	check_time_tests(void)
{
  check_time_common();
  /* XXX continue tests */
}

#define RTC_STATUSA	 0x0a	/* status register A */
#define  RTCSA_TUP	 0x80	/* time update, don't look now */
#define  RTCSA_DIVIDER   0x20   /* divider correct for 32768 Hz */
#define  RTCSA_8192      0x03
#define  RTCSA_4096      0x04
#define  RTCSA_2048      0x05
#define  RTCSA_1024      0x06
#define  RTCSA_512       0x07
#define  RTCSA_256       0x08
#define  RTCSA_128       0x09
#define  RTCSA_64        0x0a
#define  RTCSA_32        0x0b

#define RTC_STATUSB	0x0b	/* status register B */
#define	 RTCSB_DST	 0x01	/* Daylight Savings Time enable	*/
#define	 RTCSB_24HR	 0x02	/* 0 = 12 hours, 1 = 24	hours */
#define	 RTCSB_BCD	 0x04	/* 0 = BCD, 1 =	Binary coded time */
#define	 RTCSB_SQWE	 0x08	/* 1 = output sqare wave at SQW	pin */
#define	 RTCSB_UINTR	 0x10	/* 1 = enable update-ended interrupt */
#define	 RTCSB_AINTR	 0x20	/* 1 = enable alarm interrupt */
#define	 RTCSB_PINTR	 0x40	/* 1 = enable periodic clock interrupt */
#define  RTCSB_HALT      0x80	/* stop clock updates */

#define RTC_INTR	0x0c	/* status register C (R) interrupt source */
#define  RTCIR_UPDATE	 0x10	/* update intr */
#define  RTCIR_ALARM	 0x20	/* alarm intr */
#define  RTCIR_PERIOD	 0x40	/* periodic intr */
#define  RTCIR_INT	 0x80	/* interrupt output signal */

#undef MASTER_PORT_A
#undef MASTER_PORT_B
#undef SLAVE_PORT_A
#undef SLAVE_PORT_B

#define MASTER_PORT_A           0x20
#define MASTER_PORT_B           0x21
#define SLAVE_PORT_A            0xa0
#define SLAVE_PORT_B            0xa1

static unsigned char check_reg_read(unsigned char reg)
{
  t_uint8	v;

  OUTB(0x70, reg);
  INB(0x71, v);
  return v;
}

static void check_reg_write(unsigned char reg, unsigned char val)
{
  OUTB(0x70, reg);
  OUTB(0x71, val);
}

static void check_enable_irq(t_uint8 irq)
{
  t_uint8		mask;

  if (irq < 8)
    {
      INB(MASTER_PORT_B, mask);
      OUTB(MASTER_PORT_B, mask & ~(1 << irq));
    }
  else
    {
      INB(SLAVE_PORT_B, mask);
      OUTB(SLAVE_PORT_B, mask & ~(1 << (irq - 8)));
    }
}

static unsigned long ticks;

static void	rtc_irq(void)
{
  t_uint8	mask;

  ticks += 3125;

  asm volatile ("movb $0xc, %al\n\t"
                "outb %al,$0x70\n\t"
                "outb %al,$0x80\n\t"
		"inb  $0x71,%al\n\t");

  INB(SLAVE_PORT_B, mask);
  OUTB(SLAVE_PORT_B, mask);
  OUTB(SLAVE_PORT_A, 0x60 + (8 & 0x7));
  OUTB(MASTER_PORT_A, 0x60 + 2);
}

unsigned long check_rtc_ticks(void)
{
  return ticks / 100;
}

void check_rtc_init(void)
{
  event_reserve(32 + 8,
		EVENT_FUNCTION,
		EVENT_HANDLER(rtc_irq), 0);

  check_enable_irq(8);

  while (check_reg_read(RTC_STATUSA) & RTCSA_TUP)
    ; // wait till RTC ready

  ticks = 0;

  // set divider to 1024 Hz
  check_reg_write(RTC_STATUSA, RTCSA_DIVIDER | RTCSA_32);

  // set up interrupt
  check_reg_write(RTC_STATUSB, check_reg_read(RTC_STATUSB) | RTCSB_PINTR | RTCSB_SQWE);

  // reset
  check_reg_read(RTC_INTR);
}
