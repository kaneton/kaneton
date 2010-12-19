/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/clock/clock.c
 *
 * created       julien quintard   [wed nov 24 18:40:55 2010]
 * updated       julien quintard   [sat dec 18 20:59:41 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this manager provides functionalities related to the current date and time.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(clock);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the clock manager.
 */

m_clock*		_clock = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function displays the attributes of the given clock's state.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) display the clock's attributes.
 * 2) call the machine.
 */

t_error			clock_show(s_clock*			clock,
				   mt_margin			margin)
{
  /*
   * 0)
   */

  if (clock == NULL)
    CORE_ESCAPE("the 'clock' argument is null");

  /*
   * 1)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "clock:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  core: year(%u) month(%u) day(%u) hour(%u) minute(%u) "
	      "second(%u) millisecond(%u)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      clock->year,
	      clock->month,
	      clock->day,
	      clock->hour,
	      clock->minute,
	      clock->second,
	      clock->millisecond);

  /*
   * 2)
   */

  if (machine_call(clock, show, clock, margin) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function must be called in order to update the clock by the given
 * number of milliseconds.
 *
 * this is the reponsability of the timer manager to update the clock on
 * a regular basis.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			clock_update(t_uint32			millisecond)
{
  /*
   * 1)
   */

  if (machine_call(clock, update, millisecond) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function returns, through the given clock structure, the current
 * date and time.
 *
 * since this functionality depends on the hardware internals, this
 * operation is left to the machine.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) call the machine.
 */

t_error			clock_current(s_clock*			clock)
{
  /*
   * 0)
   */

  if (clock == NULL)
    CORE_ESCAPE("the 'clock' argument is null");

  /*
   * 1)
   */

  if (machine_call(clock, current, clock) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function initializes the clock manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) allocate and initialize memory for the manager's structure.
 * 3) call the machine.
 */

t_error			clock_initialize(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the clock manager\n");

  /*
   * 2)
   */

  if ((_clock = malloc(sizeof (m_clock))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the clock manager's "
		"structure");

  memset(_clock, 0x0, sizeof (m_clock));

  /*
   * 3)
   */

  if (machine_call(clock, initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the clock manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 * 3) release the manager's structure memory.
 */

t_error			clock_clean(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the clock manager\n");

  /*
   * 2)
   */

  if (machine_call(clock, clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  free(_clock);

  CORE_LEAVE();
}
