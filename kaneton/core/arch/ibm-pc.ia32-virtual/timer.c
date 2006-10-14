/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/timer.c
 *
 * created       renaud voltz   [tue feb 28 02:24:05 2006]
 * updated       matthieu bucchianeri   [fri aug 18 17:38:53 2006]
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

/*								    [cut] k3 */

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
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_timer_init,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

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

/*								   [cut] /k3 */
