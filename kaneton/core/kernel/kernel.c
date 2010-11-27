/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kernel/kernel.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sun may  6 18:14:55 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file describes the kernel manager.
 *
 * the manager does not manage a specific kaneton object but rather
 * the whole kernel.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * init variable received from the bootloader specifying segments, regions,
 * physical memory layout etc.
 */

extern t_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the kernel manager variable.
 */

m_kernel*		_kernel = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function simply initializes the kernel manager by initializing
 * every manager.
 *
 * steps:
 *
 * 1) allocate and initialize the kernel structure.
 * 2) initialize the id manager.
 * 3) initialize the set manager.
 * 4) initialize the address space manager.
 * 5) initialize the segment manager.
 * 6) initialize the region manager.
 * 7) initialize the map manager
 * 8) initialize the task manager.
 * 9) now that everything is ready, set up the fine grain memory allocator
 *    in order to work with the kernel managers rather than the
 *    pre-allocated memory area provided by the boot loader.
 * 10) initialize the thread manager.
 * 11) initialize the event manager.
 * 12) initialize the timer manager.
 * 13) initialize the clock manager.
 * 14) initialize the io manager.
 * 15) initialize the message manager.
 * 16) initialize the capability manager.
 * 17) initialize the cpu manager.
 * 18) initialize the scheduler manager.
 */

t_error			kernel_initialize(void)
{
  /*
   * 1)
   */

  if ((_kernel = malloc(sizeof(m_kernel))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the kernel "
		"manager structure");

  memset(_kernel, 0x0, sizeof(m_kernel));

  // XXX srand(time(0)) // init from the number of ticks for example

  _kernel->machine = 0;
  /*rand() %*/ //2^64; // XXX bien sur ca c'est foireux car
  // rand ne genere pas sur 64-bit

  // XXX kernel->node = ...

  _kernel->task = ID_UNUSED;
  _kernel->as = ID_UNUSED;
  _kernel->thread = ID_UNUSED;

  /*
   * 2)
   */

  module_call(console, console_message,
	      '+', "initializing the identifier manager\n");

  if (id_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the identifier manager");

  /*
   * 3)
   */

  module_call(console, console_message,
	      '+', "initializing the set manager\n");

  if (set_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the set manager");

  /*
   * 4)
   */

  module_call(console, console_message,
	      '+', "initializing the address space manager\n");

  if (as_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the address space manager");

  /*
   * 5)
   */

  module_call(console, console_message,
	      '+', "initializing the segment manager\n");

  if (segment_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the segment manager");

  /*
   * 6)
   */

  module_call(console, console_message,
	      '+', "initializing the region manager\n");

  if (region_initialize(GLUE_REGION_BASE,
			GLUE_REGION_SIZE) != ERROR_OK)
    CORE_ESCAPE("unable to initialize the region manager");

  /*
   * 7)
   */

  module_call(console, console_message,
	      '+', "initializing the map manager\n");

  if (map_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the map manager");

  /*
   * 8)
   */

  module_call(console, console_message,
	      '+', "initializing the task manager\n");

  if (task_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the task manager");

  /*
   * 9)
   */

  alloc_setup();

  /*
   * 10)
   */

  module_call(console, console_message,
	      '+', "initializing the thread manager\n");

  if (thread_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the thread manager");

  /*
   * 11)
   */

  module_call(console, console_message,
	      '+', "initializing the event manager\n");

  if (event_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the event manager");

  /*
   * 12)
   */

  module_call(console, console_message,
	      '+', "initializing the timer manager\n");

  if (timer_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the timer manager");

  /*
   * 13)
   */

  module_call(console, console_message,
	      '+', "initializing the clock manager\n");

  if (clock_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the clock manager");

  /*
   * 14)
   */

  module_call(console, console_message,
	      '+', "initializing the io manager\n");

  if (io_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the io manager");

  /*
   * 15)
   */

  module_call(console, console_message,
	      '+', "initializing the message manager\n");

  if (message_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the message manager");

  /*
   * 16)
   */

  module_call(console, console_message,
	      '+', "initializing the capability manager\n");

  if (capability_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the capability manager");

  /*
   * 17)
   */

  module_call(console, console_message,
	      '+', "initializing the cpu manager\n");

  if (cpu_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the cpu manager");

  /*
   * 18)
   */

  module_call(console, console_message,
	      '+', "initializing the scheduler manager\n");

  if (scheduler_initialize() != ERROR_OK)
    CORE_ESCAPE("unable to initialize the scheduler manager");

  CORE_LEAVE();
}

/*
 * this function cleans the kernel manager by cleaning every manager.
 *
 * steps:
 *
 * 1) cleans the scheduler manager.
 * 2) cleans the capability manager.
 * 3) cleans the message manager.
 * 4) cleans the io manager.
 * 5) cleans the clock manager.
 * 6) cleans the timer manager.
 * 7) cleans the event manager.
 * 8) cleans the cpu manager.
 * 9) cleans the thread manager.
 * 10) cleans the task manager.
 * 11) cleans the map manager.
 * 12) cleans the segment manager.
 * 13) cleans the region manager.
 * 14) cleans the address space manager.
 * 15) cleans the set manager.
 * 16) cleans the identifier manager.
 * 17) frees the kernel manager's structure.
 */

t_error			kernel_clean(void)
{
  /*
   * 1)
   */

  module_call(console, console_message,
	      '+', "cleaning the scheduler manager\n");

  if (scheduler_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the scheduler manager");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '+', "cleaning the capability manager\n");

  if (capability_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the capability manager");

  /*
   * 3)
   */

  module_call(console, console_message,
	      '+', "cleaning the message manager\n");

  if (message_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the message manager");

  /*
   * 4)
   */

  module_call(console, console_message,
	      '+', "cleaning the scheduler manager\n");

  if (io_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the io manager");

  /*
   * 5)
   */

  module_call(console, console_message,
	      '+', "cleaning the timer manager\n");

  if (timer_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the timer manager");

  /*
   * 6)
   */

  module_call(console, console_message,
	      '+', "cleaning the event manager\n");

  if (event_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the event manager");

  /*
   * 7)
   */

  module_call(console, console_message,
	      '+', "cleaning the cpu manager\n");

  if (cpu_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the cpu manager");

  /*
   * 8)
   */

  module_call(console, console_message,
	      '+', "cleaning the thread manager\n");

  if (thread_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the thread manager");

  /*
   * 9)
   */

  module_call(console, console_message,
	      '+', "cleaning the task manager\n");

  if (task_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the task manager");

  /*
   * 10)
   */

  module_call(console, console_message,
	      '+', "cleaning the map manager\n");

  if (map_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the map manager");

  /*
   * 11)
   */

  module_call(console, console_message,
	      '+', "cleaning the region manager\n");

  if (region_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the region manager");

  /*
   * 12)
   */

  module_call(console, console_message,
	      '+', "cleaning the segment manager\n");

  if (segment_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the segment manager");

  /*
   * 13)
   */

  module_call(console, console_message,
	      '+', "cleaning the address space manager\n");

  if (as_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the address space manager");

  /*
   * 14)
   */

  module_call(console, console_message,
	      '+', "cleaning the set manager\n");

  if (set_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the set manager");

  /*
   * 15)
   */

  module_call(console, console_message,
	      '+', "cleaning the identifier manager\n");

  if (id_clean() != ERROR_OK)
    CORE_ESCAPE("unable to clean the identifier manager");

  /*
   * 16)
   */

  free(_kernel);

  CORE_LEAVE();
}
