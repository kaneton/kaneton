/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/crt/crt.c
 *
 * created       matthieu bucchianeri   [mon aug  6 00:22:07 2007]
 * updated       matthieu bucchianeri   [mon aug  6 00:30:31 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

static i_task	taskid;

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * extern declaration of the program entry point.
 */

int	main(int argc, char** argv, char** envp);

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * entry point.
 *
 * the task & as id are passed and the task id of the service and driver
 * manager (mod).
 *
 * initialize the console.
 */

int	_start(i_task		task,
	       i_as		as,
	       i_task		mod)
{
  int	res;

  taskid = task;

  /*
   * initialize printf.
   */

  _crt_attach_cons();

  /*
   * call entry point.
   */

  res = main(0, NULL, NULL);

  return res;
}

/*
 * return the task identifier.
 */

i_task	_crt_get_task_id(void)
{
  return taskid;
}

/*
 * attach printf to the console driver.
 */

void	_crt_attach_cons(void)
{
  /* XXX */
}
