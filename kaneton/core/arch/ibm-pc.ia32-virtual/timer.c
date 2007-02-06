/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/timer.c
 *
 * created       renaud voltz   [tue feb 28 02:24:05 2006]
 * updated       matthieu bucchianeri   [tue feb  6 23:17:11 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the timer managment on ia32 with
 * paging architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_timer*		timer;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ia32 timer manager dispatch.
 */

d_timer				timer_dispatch =
  {

/*								   [cut] k2 */

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_timer_init,
    NULL,

/*								   [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*								   [cut] k2 */

/*
 * init the timer on the ia32-virtual architecture.
 */

t_error			ia32_timer_init(void)
{
  TIMER_ENTER(timer);

  if (pit_init(1000 / TIMER_MS_PER_TICK) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  if (event_reserve(32, EVENT_FUNCTION, EVENT_HANDLER(timer_handler))
      != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*								   [cut] /k2 */
