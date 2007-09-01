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
 * updated       matthieu bucchianeri   [sat sep  1 19:34:25 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>
#define CONS_SIMPLE_SPAWN_INTERFACE
#define CONS_SIMPLE_INLINE_INTERFACE
#include "../../drivers/cons-simple/cons-simple-driver.h"
#define MOD_SPAWN_INTERFACE
#define MOD_INLINE_INTERFACE
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

  _alloc_init();

  /*
   * attach to mod service.
   */

  if (mod_init() != ERROR_NONE)
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

  /* XXX not safe to return */

  printf("warning: program terminated.\n");

  while (1)
    ;

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
 * return the mod service identifier.
 */

i_task	_crt_get_mod_id(void)
{
  return modid;
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

      cons_simple_write(print_console, print_n, print_buffer, NULL);

      print_n = 0;
    }
  return (1);
}

/*
 * attach printf to the console driver.
 */

void	_crt_attach_cons_to(i_task	cons)
{
  if (cons_simple_init() != ERROR_NONE)
    {
      /* XXX fatal error */
    }

  print_console = cons;
  printf_init(crt_printchar, NULL);
}

/*
 * attach printf to the console driver.
 */

void	_crt_attach_cons(void)
{
  if (mod_get_service(modid, "cons-simple", &print_console) != ERROR_NONE)
    {
      /* XXX fatal error */
    }

  if (cons_simple_init() != ERROR_NONE)
    {
      /* XXX fatal error */
    }

  printf_init(crt_printchar, NULL);
}
