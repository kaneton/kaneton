/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/machine/platform/ibm-pc/pit.c
 *
 * created       renaud voltz   [mon feb 27 04:20:33 2006]
 * updated       julien quintard   [fri jan 14 20:12:22 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the management of the PIT - Programmable Interval
 * Timer 8254 on ibmpc.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <platform/platform.h>
#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * initialize the PIT 8254.
 *
 * steps:
 *
 * 1) calculate and check counter limit for the given frequency.
 * 2) the PIT 8254 interprets value 0 as 65536.
 * 3) setup the PIT0 to run in mode 2 (rate generator).
 */

t_status		platform_pit_initialize(void)
{
  t_uint32	latch;

  /*
   * 1)
   */

  latch = PLATFORM_PIT_RATE / (1000 / TIMER_DELAY);

  if (latch == 0 || latch > 65536)
    MACHINE_ESCAPE("the computed latch is invalid");

  /*
   * 2)
   */

  if (latch == 65536)
    latch = 0;

  /*
   * 3)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIT_8254_CTRL,
			0x34);

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIT_0,
			latch & 0xFF);

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIT_0,
			(latch >> 8) & 0xFF);

  MACHINE_LEAVE();
}

/*
 * this function cleans the PIT.
 */

t_status		platform_pit_clean(void)
{
  MACHINE_LEAVE();
}
