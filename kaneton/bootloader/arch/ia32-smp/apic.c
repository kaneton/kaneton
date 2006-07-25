/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ia32-smp/apic.c
 *
 * created       matthieu bucchianeri   [tue jul 25 11:22:27 2006]
 * updated       matthieu bucchianeri   [tue jul 25 16:14:28 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * needed to convert nanoseconds to APIC timer ticks.
 */

static t_uint64		timeref = 0;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function calibrates the APIC timer.
 *
 * steps:
 *
 * 1) setup the timer for one shot with a divisor equal to 1.
 * 2) setup the initial count to a big number.
 * 3) wait for the sample amout of time.
 * 4) compute the time reference.
 */

void			bootloader_apic_calibrate_timer(void)
{
  t_sint32		t1;
  t_sint32		t2;
  int			i;

  /*
   * 1)
   */

  apic_write(APIC_REG_DIV, 11);
  apic_write(APIC_REG_TIMER_CONF, 1 << 17);

  /*
   * 2)
   */

  apic_write(APIC_REG_TIMER, 1000000000);

  /*
   * 3)
   */

  t1 = apic_read(APIC_REG_COUNT);
  for (i = 0; i < 10000000; i++)
    ;

  t2 = apic_read(APIC_REG_COUNT);

  /*
   * 4)
   */

  timeref = t1 - t2;
}

/*
 * this function waits for the given amount of nanoseconds.
 *
 * steps:
 *
 * 1) compute the tick count.
 * 2) setup the timer.
 * 3) wait until the delay expires.
 */

void			bootloader_apic_wait(t_uint32			delay)
{
  t_uint64		total;

  /*
   * 1)
   */

  total = (delay * timeref) / 4000000;

  /*
   * 2)
   */

  apic_write(APIC_REG_DIV, 11);
  apic_write(APIC_REG_TIMER_CONF, 1 << 17);
  apic_write(APIC_REG_TIMER, (t_uint32)total);

  /*
   * 3)
   */

  while (apic_read(APIC_REG_COUNT))
    ;
}
