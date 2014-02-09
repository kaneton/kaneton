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
 * updated       julien quintard   [fri jan 28 14:07:21 2011]
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

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task dispatcher.
 */

d_task			glue_task_dispatch =
  {
    glue_task_show,
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
    NULL,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows the given task's machine-related data.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) display the machine-specific information.
 */

t_status		glue_task_show(i_task			id,
				       mt_margin		margin)
{
  o_task*		o;

  /*
   * 1)
   */

  if (task_get(id, &o) != STATUS_OK)
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
	      "    I/O: map(0x%x) flush(%s)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      &o->machine.io.map,
	      o->machine.io.flush == BOOLEAN_TRUE ? "true" : "false");

  MACHINE_LEAVE();
}

/*
 * this function initializes the I/O map in order to terminate the
 * task reservation process.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) clear the I/O map.
 */

t_status		glue_task_reserve(t_class		class,
					  t_behaviour		behav,
					  t_priority		prior,
					  i_task*		id)
{
  o_task*		o;

  /*
   * 1)
   */

  if (task_get(*id, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (architecture_io_clear(*id) != STATUS_OK)
    MACHINE_ESCAPE("unable to clear the I/O bitmap");

  o->machine.io.flush = BOOLEAN_FALSE;

  MACHINE_LEAVE();
}
