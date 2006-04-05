/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/kaneton/core/arch/ia32-virtual/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       renaud voltz   [tue apr  4 03:08:03 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX THREAD assignments need to be written.
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

d_thread                         thread_dispatch =
  {
    NULL,
    ia32_thread_suspend,
    ia32_thread_execute,
    ia32_thread_clone,
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


/*                                                                 [cut] /k4 */
