/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/kaneton/timer/timer.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       renaud voltz   [sun feb 12 23:04:54 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the timer manager.
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

machdep_include(timer);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the timer manager variable.
 */

m_timer*                 timer = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given timer.
 */

t_error			timer_show(t_timerid			timerid)
{
  TIMER_ENTER(timer);


  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * this function dumps the timers managed by the kernel.
 */

t_error			timer_dump(void)
{
  TIMER_ENTER(timer);


  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * notify every subscribed task that an timer occured.
 */

t_error			timer_notify(t_timerid			timerid)
{
  TIMER_ENTER(timer);


  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * reserve an timer.
 */

t_error			timer_reserve(t_timerid			timerid)
{
  TIMER_ENTER(timer);


  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * this function releases an timer.
 *
 */

t_error			timer_release(t_timerid			timerid)
{
  TIMER_ENTER(timer);


  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * assign a task to an timer.
 */

t_error			timer_subscribe(t_timerid		timerid,
					t_tskid			tskid)
{
  TIMER_ENTER(timer);


  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * unassociate a task from an timer.
 */

t_error			timer_unsubscribe(t_timerid		timerid,
					  t_tskid		tskid)
{
  TIMER_ENTER(timer);


  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * this function get an timer object from the timer container.
 */

t_error			timer_get(t_timerid			timerid,
				  o_timer**			o)
{
  TIMER_ENTER(timer);


  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * initialize the timer manager.
 */

t_error			timer_init(void)
{


  return ERROR_NONE;
}

/*
 * destroy the timer manager.
 */

t_error			timer_clean(void)
{


  return ERROR_NONE;
}

/*                                                                  [cut] k3 */
