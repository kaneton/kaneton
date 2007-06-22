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

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the kernel manager variable.
 */

m_kernel*		kernel = NULL;

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * init variable received from the bootloader specifying segments, regions,
 * physical memory layout etc.
 */

extern t_init*		init;

/*
 * kernel address space identifier
 */

extern i_as		kasid;

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

  if ((kernel = malloc(sizeof(m_kernel))) == NULL)
    cons_msg('!', "kernel: cannot allocate memory for the kernel manager "
	     "structure\n");

  memset(kernel, 0x0, sizeof(m_kernel));

  // XXX srand(time(0)) // init from the number of ticks for example

  kernel->machine = /*rand() %*/ 2^64; // XXX bien sur ca c'est foireux car
				   // rand ne genere pas sur 64-bit

  // XXX kernel->node = ...

  /*
   * 2)
   */

  cons_msg('+', "starting id manager\n");

  if (id_initialize() != ERROR_NONE)
    core_error("cannot initialize the id manager\n");

  /*
   * 3)
   */

  cons_msg('+', "starting set manager\n");

  if (set_initialize() != ERROR_NONE)
    core_error("cannot initialize the set manager\n");

  /*
   * 4)
   */

  cons_msg('+', "starting as manager\n");

  if (as_initialize() != ERROR_NONE)
    core_error("cannot initialize the address space manager\n");

  /*
   * 5)
   */

  cons_msg('+', "starting segment manager\n");

  if (segment_initialize() != ERROR_NONE)
    core_error("cannot initialize the segment manager\n");

  /*
   * 6)
   */

  cons_msg('+', "starting region manager\n");

  if (region_initialize(REGION_VMEM_MIN, REGION_VMEM_MAX -
			REGION_VMEM_MIN) != ERROR_NONE)
    core_error("cannot initialize the region manager\n");

  /*
   * 7)
   */

  cons_msg('+', "starting map manager\n");

  if (map_initialize() != ERROR_NONE)
    core_error("cannot initialize the map manager\n");

  /*
   * 8)
   */

  cons_msg('+', "starting task manager\n");

  if (task_initialize() != ERROR_NONE)
    core_error("cannot initialize the task manager\n");

  alloc_setup(map_reserve, map_release, kasid);

  /*
   * 9)
   */

  cons_msg('+', "starting thread manager\n");

  if (thread_initialize() != ERROR_NONE)
    core_error("cannot initialize the thread manager\n");

  /*
   * 10)
   */

  cons_msg('+', "starting event manager\n");

  if (event_initialize() != ERROR_NONE)
    core_error("cannot initialize the event manager\n");

  /*
   * 11)
   */

  cons_msg('+', "starting timer manager\n");

  if (timer_initialize() != ERROR_NONE)
    core_error("cannot initialize the timer manager\n");

  /*
   * 12)
   */

  cons_msg('+', "starting io manager\n");

  if (io_initialize() != ERROR_NONE)
    core_error("cannot initialize the io manager\n");

  /*
   * 13)
   */

  cons_msg('+', "starting message manager\n");

  if (message_initialize() != ERROR_NONE)
    core_error("cannot initialize the message manager\n");

  /*
   * 14)
   */

  cons_msg('+', "starting capability manager\n");

  if (capability_initialize() != ERROR_NONE)
    core_error("cannot initialize the capability manager\n");

  /*
   * 15)
   */

  cons_msg('+', "starting cpu manager\n");

  if (cpu_initialize() != ERROR_NONE)
    core_error("cannot initialize the cpu manager\n");

  /*
   * 16)
   */

  cons_msg('+', "starting scheduler manager\n");

  if (scheduler_initialize() != ERROR_NONE)
    core_error("cannot initialize the scheduler manager\n");
}

/*
 * this function cleans the kernel manager by cleaning every manager.
 */

void			kernel_clean(void)
{
  scheduler_clean();

/*								    [cut] k1 */

  capability_clean();

/*								   [cut] /k1 */

  message_clean();

/*								    [cut] k1 */

  io_clean();

/*								   [cut] /k1 */

  timer_clean();

  event_clean();

/*								    [cut] k1 */

  cpu_clean();

/*								   [cut] /k1 */

  thread_clean();

  task_clean();

  map_clean();

  region_clean();

  segment_clean();

  as_clean();

  set_clean();

  id_clean();
}
