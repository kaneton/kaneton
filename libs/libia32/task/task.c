/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/task/task.c
 *
 * created       renaud voltz   [tue apr  4 21:45:07 2006]
 * updated       renaud voltz   [tue apr  4 21:45:07 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX THREAD assigments have to be written.
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
 * ---------- functions -------------------------------------------------------
 */

/*
 *
 */

t_error			context_save(t_uint16*			ss,
				     t_uint32*			esp)
{
  CONTEXT_SAVE(ss, esp);

  return ERROR_NONE;
}

/*
 *
 */

t_error			context_restore(t_uint16		ss,
					t_uint32		esp)
{
  CONTEXT_RESTORE(ss, esp);

  return ERROR_NONE;
}
