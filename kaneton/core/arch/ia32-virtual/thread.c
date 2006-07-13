/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/arch/ia32-virtual/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       julien quintard   [sat jul  8 02:30:44 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k4 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread manager dispatch.
 */

d_thread			thread_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 *
 */

t_error			ia32_thread_suspend(t_thrid		threadid)
{

  return ERROR_NONE;
}

/*
 *
 */

t_error			ia32_thread_execute(t_thrid		threadid)
{

  return ERROR_NONE;
}

/*
 *
 */

t_error			ia32_thread_clone(t_thrid		threadid)
{

  return ERROR_NONE;
}

/*
 *
 */

t_error			ia32_thread_reserve()
{

}

/*                                                                 [cut] /k4 */
