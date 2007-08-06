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
 * updated       matthieu bucchianeri   [mon aug  6 17:57:32 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>
#include "../../drivers/cons-simple/cons-simple-driver.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PRINT_BUFFER	CONS_SIMPLE_MAX_BUFFER

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * store the current task & as id.
 */

static i_task	taskid;
static i_as	asid;

/*
 * store the current print status.
 */

static i_task	print_console = ID_UNUSED;
static int	print_n = 0;
static char	print_buffer[PRINT_BUFFER];

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * entry point.
 *
 * the task & as id are passed and the task id of the service and driver
 * manager (mod).
 *
 * initialize lot of things before calling the main().
 */

int		_start(i_task		task,
		       i_as		as,
		       i_task		mod)
{
  int		res;
  t_vaddr	page;

  taskid = task;
  asid = as;

  /*
   * initialize malloc();
   */

  if (map_reserve(as, MAP_OPT_NONE, 4096, PERM_READ | PERM_WRITE,
		  &page) != ERROR_NONE)
    {
      /* XXX throw fatal error */
    }

  alloc_init(page, 4096);

  alloc_setup(map_reserve, map_release, as);

  /*
   * initialize printf().
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
 * return the as identifier.
 */

i_as	_crt_get_as_id(void)
{
  return asid;
}

/*
 * print one character (buffered).
 */

static int	crt_printchar(char c)
{
  if (c != '\n' && print_n < PRINT_BUFFER - 1 && c != -1)
    print_buffer[print_n++] = c;
  else
    {
      if (c != -1)
	print_buffer[print_n++] = c;

      if (print_console == ID_UNUSED)
	{
	  if (print_n < PRINT_BUFFER - 1)
	    return (1); /* we try to buffer */

	  /* XXX fatal error */
	}

      {
	t_driver_cons_simple_message*	message;
	i_node				cons_driver;

	cons_driver.machine = 0;
	cons_driver.task = print_console;

	if ((message = malloc(sizeof (message) + PRINT_BUFFER)) == NULL)
	  {
	    /* XXX fatal error */
	  }

	message->u.request.operation = CONS_SIMPLE_DRIVER_WRITE;
	message->u.request.u.write.count = print_n;
	memcpy(&message->u.request.u.write.c[0], print_buffer, print_n);

	if (message_send(cons_driver,
			 MESSAGE_TYPE_DRIVER_CONS_SIMPLE, (t_vaddr)message,
			 sizeof (*message) + PRINT_BUFFER) != ERROR_NONE)
	  {
	    /* XXX fatal error */
	  }

	free(message);
      }

      print_n = 0;
    }
  return (1);
}

/*
 * attach printf to the console driver.
 */

void	_crt_attach_cons(void)
{
  /* print_console = XXX get cons service id */
  print_console = 2;

  if (message_register(MESSAGE_TYPE_DRIVER_CONS_SIMPLE,
		       MESSAGE_SIZE_DRIVER_CONS_SIMPLE) != ERROR_NONE)
    {
      /* XXX fatal error */
    }

  printf_init(crt_printchar, NULL);
}
