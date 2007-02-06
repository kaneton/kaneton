/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/time/timer.c
 *
 * created       renaud voltz   [mon feb 27 04:20:33 2006]
 * updated       matthieu bucchianeri   [tue feb  6 19:43:50 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this  file  implements   the  management  of  the   progammable   interval
 * timer  (PIT 8254)  on  ia32  architecture.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k2 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * initialize the pit 8254.
 *
 * steps:
 *
 * 1) calculate and check counter limit for the given frequency.
 * 2) the pit 8254 interprets value 0 as 65536.
 * 3) setup the timer0 to run in mode 2 (rate generator).
 */

t_error			pit_init(t_uint32			frequency)
{
  t_uint32	latch;

  /*
   * 1)
   */

  latch = CLOCK_TICK_RATE / frequency;

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

  OUTB(PIT_8254_CTRL, 0x34);
  OUTB(TIMER_0, latch & 0xFF);
  OUTB(TIMER_0, (latch >> 8) & 0xFF);

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
