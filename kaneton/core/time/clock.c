/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/core/time/clock.c
 *
 * created       julien quintard   [wed nov 24 18:40:55 2010]
 * updated       julien quintard   [sat dec  4 22:50:13 2010]
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
 * this function displays the attributes of the given clock.
 */

t_error			clock_show(t_clock*			clock)
{
  module_call(console, message,
	      '#', "dumping the clock:\n");

  module_call(console, message,
	      '#', "  millisecond: %u\n",
	      clock->millisecond);

  module_call(console, message,
	      '#', "  second: %u\n",
	      clock->second);

  module_call(console, message,
	      '#', "  minute: %u\n",
	      clock->minute);

  module_call(console, message,
	      '#', "  hours: %u\n",
	      clock->hour);

  module_call(console, message,
	      '#', "  day: %u\n",
	      clock->day);

  module_call(console, message,
	      '#', "  month: %u\n",
	      clock->month);

  module_call(console, message,
	      '#', "  year: %u\n",
	      clock->year);

  CORE_LEAVE();
}

/*
 * this function must be called in order to update the clock by
 * the given milliseconds.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			clock_update(t_uint32			millisecond)
{
  if (machine_call(clock, clock_update, millisecond) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function returns, through the given clock structure, the current
 * date and time.
 *
 * steps:
 *
 * 1) since this functionality depends on the hardware internals, this
 *    function simply calls the machine.
 */

t_error			clock_current(t_clock*			clock)
{
  /*
   * 1)
   */

  if (machine_call(clock, clock_current, clock) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function initializes the clock manager.
 *
 * steps:
 *
 * 1) allocate and initialize memory for the manager's structure.
 * 2) call the machine.
 */

t_error			clock_initialize(void)
{
  /*
   * 1)
   */

  if ((_clock = malloc(sizeof(m_clock))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the clock manager's "
		"structure");

  memset(_clock, 0x0, sizeof(m_clock));

  /*
   * 2)
   */

  if (machine_call(clock, clock_initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the clock manager.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) release the manager's structure memory.
 */

t_error			clock_clean(void)
{
  /*
   * 1)
   */

  if (machine_call(clock, clock_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  free(_clock);

  CORE_LEAVE();
}
