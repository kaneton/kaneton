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
 * 1) allocates and initializes the kernel manager structure.
 * 2) initializes the id manager.
 * 3) initializes the set manager.
 * 4) initializes the address space manager.
 * 5) initializes the segment manager.
 * 6) initializes the region manager.
 * 7) initializes the map manager
 * 8) initializes the task manager.
 * 9) initializes the thread manager.
 * 10) initializes the event manager.
 * 11) initializes the timer manager.
 * 12) initializes the io manager.
 * 13) initializes the message manager.
 * 14) initializes the capability manager.
 * 15) initialize the cpu manager.
 * 16) initialize the scheduler manager.
 */

void			kernel_initialize(void)
{
  /*
   * 1)
   */

  if ((_kernel = malloc(sizeof(m_kernel))) == NULL)
    kernel_error("unable to allocate memory for the kernel "
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
    kernel_error("unable to initialize the identifier manager");

  /*
   * 3)
   */

  module_call(console, console_message,
	      '+', "initializing the set manager\n");

  if (set_initialize() != ERROR_OK)
    kernel_error("unable to initialize the set manager");

  /*
   * 4)
   */

  module_call(console, console_message,
	      '+', "initializing the address space manager\n");

  if (as_initialize() != ERROR_OK)
    kernel_error("unable to initialize the address space manager");

  /*
   * 5)
   */

  module_call(console, console_message,
	      '+', "initializing the segment manager\n");

  if (segment_initialize() != ERROR_OK)
    kernel_error("unable to initialize the segment manager");

  /*
   * 6)
   */

  module_call(console, console_message,
	      '+', "initializing the region manager\n");

  if (region_initialize(REGION_VMEM_MIN, REGION_VMEM_MAX -
			REGION_VMEM_MIN) != ERROR_OK)
    kernel_error("unable to initialize the region manager");

  /*
   * 7)
   */

  module_call(console, console_message,
	      '+', "initializing the map manager\n");

  if (map_initialize() != ERROR_OK)
    kernel_error("unable to initialize the map manager");

  /*
   * 8)
   */

  module_call(console, console_message,
	      '+', "initializing the task manager\n");

  if (task_initialize() != ERROR_OK)
    kernel_error("unable to initialize the task manager");

  alloc_setup();

  /*
   * 9)
   */

  module_call(console, console_message,
	      '+', "initializing the thread manager\n");

  if (thread_initialize() != ERROR_OK)
    kernel_error("unable to initialize the thread manager");

  /*
   * 10)
   */

  module_call(console, console_message,
	      '+', "initializing the event manager\n");

  if (event_initialize() != ERROR_OK)
    kernel_error("unable to initialize the event manager");

  /*
   * 11)
   */

  module_call(console, console_message,
	      '+', "initializing the timer manager\n");

  if (timer_initialize() != ERROR_OK)
    kernel_error("unable to initialize the timer manager");

  /*
   * 12)
   */

  module_call(console, console_message,
	      '+', "initializing the io manager\n");

  if (io_initialize() != ERROR_OK)
    kernel_error("unable to initialize the io manager");

  /*
   * 13)
   */

  module_call(console, console_message,
	      '+', "initializing the message manager\n");

  if (message_initialize() != ERROR_OK)
    kernel_error("unable to initialize the message manager");

  /*
   * 14)
   */

  module_call(console, console_message,
	      '+', "initializing the capability manager\n");

  if (capability_initialize() != ERROR_OK)
    kernel_error("unable to initialize the capability manager");

  /*
   * 15)
   */

  module_call(console, console_message,
	      '+', "initializing the cpu manager\n");

  if (cpu_initialize() != ERROR_OK)
    kernel_error("unable to initialize the cpu manager");

  /*
   * 16)
   */

  module_call(console, console_message,
	      '+', "initializing the scheduler manager\n");

  if (scheduler_initialize() != ERROR_OK)
    kernel_error("unable to initialize the scheduler manager");
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
 * 5) cleans the timer manager.
 * 6) cleans the event manager.
 * 7) cleans the cpu manager.
 * 8) cleans the thread manager.
 * 9) cleans the task manager.
 * 10) cleans the map manager.
 * 11) cleans the segment manager.
 * 12) cleans the region manager.
 * 13) cleans the address space manager.
 * 14) cleans the set manager.
 * 15) cleans the identifier manager.
 * 16) frees the kernel manager's structure.
 */

void			kernel_clean(void)
{
  /*
   * 1)
   */

  module_call(console, console_message,
	      '+', "cleaning the scheduler manager\n");

  if (scheduler_clean() != ERROR_OK)
    kernel_error("unable to clean the scheduler manager");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '+', "cleaning the capability manager\n");

  if (capability_clean() != ERROR_OK)
    kernel_error("unable to clean the capability manager");

  /*
   * 3)
   */

  module_call(console, console_message,
	      '+', "cleaning the message manager\n");

  if (message_clean() != ERROR_OK)
    kernel_error("unable to clean the message manager");

  /*
   * 4)
   */

  module_call(console, console_message,
	      '+', "cleaning the scheduler manager\n");

  if (io_clean() != ERROR_OK)
    kernel_error("unable to clean the io manager");

  /*
   * 5)
   */

  module_call(console, console_message,
	      '+', "cleaning the timer manager\n");

  if (timer_clean() != ERROR_OK)
    kernel_error("unable to clean the timer manager");

  /*
   * 6)
   */

  module_call(console, console_message,
	      '+', "cleaning the event manager\n");

  if (event_clean() != ERROR_OK)
    kernel_error("unable to clean the event manager");

  /*
   * 7)
   */

  module_call(console, console_message,
	      '+', "cleaning the cpu manager\n");

  if (cpu_clean() != ERROR_OK)
    kernel_error("unable to clean the cpu manager");

  /*
   * 8)
   */

  module_call(console, console_message,
	      '+', "cleaning the thread manager\n");

  if (thread_clean() != ERROR_OK)
    kernel_error("unable to clean the thread manager");

  /*
   * 9)
   */

  module_call(console, console_message,
	      '+', "cleaning the task manager\n");

  if (task_clean() != ERROR_OK)
    kernel_error("unable to clean the task manager");

  /*
   * 10)
   */

  module_call(console, console_message,
	      '+', "cleaning the map manager\n");

  if (map_clean() != ERROR_OK)
    kernel_error("unable to clean the map manager");

  /*
   * 11)
   */

  module_call(console, console_message,
	      '+', "cleaning the region manager\n");

  if (region_clean() != ERROR_OK)
    kernel_error("unable to clean the region manager");

  /*
   * 12)
   */

  module_call(console, console_message,
	      '+', "cleaning the segment manager\n");

  if (segment_clean() != ERROR_OK)
    kernel_error("unable to clean the segment manager");

  /*
   * 13)
   */

  module_call(console, console_message,
	      '+', "cleaning the address space manager\n");

  if (as_clean() != ERROR_OK)
    kernel_error("unable to clean the address space manager");

  /*
   * 14)
   */

  module_call(console, console_message,
	      '+', "cleaning the set manager\n");

  if (set_clean() != ERROR_OK)
    kernel_error("unable to clean the set manager");

  /*
   * 15)
   */

  module_call(console, console_message,
	      '+', "cleaning the identifier manager\n");

  if (id_clean() != ERROR_OK)
    kernel_error("unable to clean the identifier manager");

  /*
   * 16)
   */

  free(_kernel);
}
