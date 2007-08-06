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
 * updated       matthieu bucchianeri   [mon aug  6 22:16:46 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>
#include "../../drivers/cons-simple/cons-simple-driver.h"
#include "../../services/mod/mod-service.h"

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
static i_task	modid;

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
		       i_task		mod,
		       int		argc,
		       char**		argv,
		       char**		envp)
{
  int		res;
  t_vaddr	page;

  taskid = task;
  asid = as;
  modid = mod;

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
   * attach to mod service.
   */

  if (message_register(MESSAGE_TYPE_SERVICE_MOD,
		       MESSAGE_SIZE_SERVICE_MOD) != ERROR_NONE)
    {
      /* XXX fatal error */
    }

  /*
   * initialize printf().
   */

  if (mod != ID_UNUSED)
    _crt_attach_cons();

  /*
   * call entry point.
   */

  res = main(argc, argv, envp);

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
	t_vsize				size;

	cons_driver.machine = 0;
	cons_driver.task = print_console;

	if ((message = malloc(sizeof (*message) + PRINT_BUFFER)) == NULL)
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

	message_receive(MESSAGE_TYPE_DRIVER_CONS_SIMPLE, (t_vaddr)message,
			&size, &cons_driver);

	free(message);
      }

      print_n = 0;
    }
  return (1);
}

/*
 * attach printf to the console driver.
 */

void	_crt_attach_cons_to(i_task	cons)
{
  message_register(MESSAGE_TYPE_DRIVER_CONS_SIMPLE,
		   MESSAGE_SIZE_DRIVER_CONS_SIMPLE);

  print_console = cons;
  printf_init(crt_printchar, NULL);
}

/*
 * attach printf to the console driver.
 */

void	_crt_attach_cons(void)
{
  {
    t_service_mod_message*	message;
    i_node			mod_service;
    t_vsize			size;

    mod_service.machine = 0;
    mod_service.task = modid;

    if ((message = malloc(sizeof (*message) + 12)) == NULL)
      {
	/* XXX fatal error */
      }

    message->u.request.operation = MOD_SERVICE_GET_SERVICE;
    strcpy(message->u.request.u.get_service.name, "cons-simple");

    if (message_send(mod_service,
		     MESSAGE_TYPE_SERVICE_MOD, (t_vaddr)message,
		     sizeof (*message) + 12) != ERROR_NONE)
      {
	/* XXX fatal error */
      }

    if (message_receive(MESSAGE_TYPE_SERVICE_MOD, (t_vaddr)message,
			&size, &mod_service) != ERROR_NONE)
      {
	/* XXX fatal error */
      }

    print_console = message->u.reply.u.get_service.id;

    free(message);
  }

  if (message_register(MESSAGE_TYPE_DRIVER_CONS_SIMPLE,
		       MESSAGE_SIZE_DRIVER_CONS_SIMPLE) != ERROR_NONE)
    {
      /* XXX fatal error */
    }

  printf_init(crt_printchar, NULL);
}
