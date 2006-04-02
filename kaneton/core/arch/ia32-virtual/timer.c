/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/arch/ia32-virtual/timer.c
 *
 * created       renaud voltz   [tue feb 28 02:24:05 2006]
 * updated       julien quintard   [sun apr  2 14:17:17 2006]
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
 * XXX TIMER assignments have to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*								    [cut] k3 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ia32 timer manager dispatch.
 */

d_timer				timer_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_timer_init,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

t_error			ia32_timer_init(void)
{
  return pit_init(100);
}

/*								   [cut] /k3 */
