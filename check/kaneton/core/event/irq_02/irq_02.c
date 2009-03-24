/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

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

static
unsigned char
reg_read(unsigned char reg)
{
  t_uint8	v;

  OUTB(0x70, reg);
  INB(0x71, v);
  return v;
}

static
void
reg_write(unsigned char reg, unsigned char val)
{
  OUTB(0x70, reg);
  OUTB(0x71, val);
}

static volatile int	thrown = 0;

static void	check_irq8(t_id	id)
{
  thrown++;

  asm volatile ("movb $0xc, %al\n\t"
                "outb %al,$0x70\n\t"
                "outb %al,$0x80\n\t"
		"inb  $0x71,%al\n\t");
}

/*
 * reserve event onto int3 and throw an int3.
 */
static void		check_event_irq_02_entry(void)
{
  int		i;

  TEST_ENTER();

  ASSERT(event_reserve(32 + 8,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_irq8), 0) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Triggering IRQ\n");

  while (reg_read(RTC_STATUSA) & RTCSA_TUP)
    ; // wait till RTC ready

  // set divider to 1024 Hz
  reg_write(RTC_STATUSA, RTCSA_DIVIDER | RTCSA_1024);

  // set up interrupt
  reg_write(RTC_STATUSB, reg_read(RTC_STATUSB) | RTCSB_PINTR | RTCSB_SQWE);

  // reset
  reg_read(RTC_INTR);

  for (i = 0; i < 100000000; i++)
    asm volatile("nop");

  ASSERT(thrown > 1, " x One or more IRQ was not caught\n");

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_event_irq_02)
