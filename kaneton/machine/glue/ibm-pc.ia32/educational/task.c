/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/glue/ibm-pc.ia32/educational/task.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [sun dec 19 18:29:22 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the task manager's glue.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task dispatcher.
 */

d_task			glue_task_dispatch =
  {
    NULL,
    NULL,
    glue_task_reserve,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_task_initialize,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX
 */

t_error			glue_task_show(i_task			id)
{
  o_task*		o;

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  machine:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  I/O: map(0x%x) flush(%s)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      &o->machine.io.map,
	      o->machine.io.flush == BOOLEAN_TRUE ? "true" : "false");

  MACHINE_LEAVE();
}

/*
 * this function reserves a task.
 */

t_error			glue_task_reserve(t_class		class,
					  t_behaviour		behav,
					  t_priority		prior,
					  i_task*		id)
{
  o_task*		o;

  /*
   * XXX
   */

  if (ia32_clear_io_bitmap(*id) != ERROR_OK)
    MACHINE_ESCAPE("unable to clear the IO bitmap");

  /*
   * XXX
   */

  if (task_get(*id, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * XXX
   */

  o->machine.io.flush = BOOLEAN_FALSE;

  MACHINE_LEAVE();
}

/*
 * this function initializes the task manager's glue.
 */

t_error			glue_task_initialize(void)
{
  /*
   * XXX
   */

  if (ia32_kernel_as_finalize() != ERROR_OK)
    MACHINE_ESCAPE("unable to finalize the address space");

  MACHINE_LEAVE();
}
