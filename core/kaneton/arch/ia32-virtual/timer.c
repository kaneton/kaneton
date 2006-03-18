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

/*								[cut] k3 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ia32 timer manager interface.
 */

i_timer				timer_interface =
  {

    ia32_timer_notify,
    ia32_timer_check,
    ia32_timer_modify,
    ia32_timer_reserve,
    ia32_timer_release,
    ia32_timer_init,
    ia32_timer_clean

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * clean the timer.
 *
 * nothing to do on the ia32 architecture.
 */

t_error			ia32_timer_notify(t_timerid		timerid)
{
  return ERROR_NONE;
}
/*
 * clean the timer.
 *
 * nothing to do on the ia32 architecture.
 */

t_error			ia32_timer_check(t_timerid		timerid,
					 t_uint32*		delay)
{
  return ERROR_NONE;
}

/*
 * clean the timer.
 *
 * nothing to do on the ia32 architecture.
 */

t_error			ia32_timer_modify(t_timerid		timerid,
					  t_uint32		delay,
					  t_uint32		repeat)
{
  return ERROR_NONE;
}

/*
 * clean the timer.
 *
 * nothing to do on the ia32 architecture.
 */

t_error			ia32_timer_reserve(t_tskid		taskid,
					   t_uint32		delay,
					   t_uint32		repeat,
					   t_timerid*		timerid)
{
  return ERROR_NONE;
}

/*
 * clean the timer.
 *
 * nothing to do on the ia32 architecture.
 */

t_error			ia32_timer_release(t_timerid		timerid)
{
  return ERROR_NONE;
}

/*
 * initialize the timer on the ia32 architecture.
 *
 * steps:
 */

t_error			ia32_timer_init(void)
{
  return pit_init(100);
}

/*
 * clean the timer.
 *
 * nothing to do on the ia32 architecture.
 */

t_error			ia32_timer_clean(void)
{
  return ERROR_NONE;
}

/*								[cut] /k3 */
