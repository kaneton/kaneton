/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/timer.c
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:41:56 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 22:50:34 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the timer managment on octane with
 * paging architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <liboctane.h>
#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

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
    NULL,
    NULL,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */
