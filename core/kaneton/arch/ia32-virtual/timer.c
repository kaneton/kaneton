/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/kaneton/arch/ia32-virtual/timer.c
 *
 * created       renaud voltz   [tue feb 28 02:24:05 2006]
 * updated       renaud voltz   [tue feb 28 02:24:05 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the timer managment on ia32 with
 * paging architecture.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX TIMER
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the timer manager interface.
 */

i_timer				timer_interface =
  {

    /*								[cut] k3 */

    ia32_timer_init,
    ia32_timer_clean
    /*								[cut] /k3 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * initialize the timer on the ia32 architecture.
 *
 * steps:
 */

t_error			ia32_timer_init(void)
{
  return pit_init(100);
}

t_error			ia32_timer_clean(void)
{
  return ERROR_NONE;
}
