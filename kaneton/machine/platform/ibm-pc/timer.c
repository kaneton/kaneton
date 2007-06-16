/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/platform/ibm-pc/timer.c
 *
 * created       renaud voltz   [mon feb 27 04:20:33 2006]
 * updated       matthieu bucchianeri   [fri jun 15 01:32:49 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the management of the programmable interval
 * timer (PIT 8254) on ibmpc.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>

#include <platform/platform.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * initialize the pit 8254.
 *
 * steps:
 *
 * 1) calculate and check counter limit for the given frequency.
 * 2) the pit 8254 interprets value 0 as 65536.
 * 3) setup the timer0 to run in mode 2 (rate generator).
 */

t_error			ibmpc_timer_init(void)
{
  t_uint32	latch;

  /*
   * 1)
   */

  latch = IBMPC_CLOCK_TICK_RATE / IBMPC_TIMER_FREQUENCY;

  if (!latch || latch > 65536)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (latch == 65536)
    latch = 0;

  /*
   * 3)
   */

  OUTB(IBMPC_PIT_8254_CTRL, 0x34);
  OUTB(IBMPC_TIMER_0, latch & 0xFF);
  OUTB(IBMPC_TIMER_0, (latch >> 8) & 0xFF);

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
