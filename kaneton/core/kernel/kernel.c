/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/kernel/kernel.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sat jun 24 17:25:31 2006]
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

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * init variable received from the bootloader specifying segments, regions,
 * physical memory layout etc..
 */

extern t_init*		init;

/*
 * the kernel manager variable.
 */

m_kernel*		kernel = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function simply initialises the kernel manager by initializing
 * every manager.
 *
 * steps:
 *
 * 1) allocates and initializes the kernel manager structure.
 * 2) if needed, initialises the stats manager.
 * 3) initialises the id manager.
 * 4) initialises the set manager.
 * 5) initialises the address space manager.
 * 6) initialises the segment manager.
 * 7) initialises the region manager.
 * 8) initialises the map manager
 * 9) initialises the task manager.
 * 10) initialises the thread manager.
 * 11) initialises the event manager.
 * 12) initialises the timer manager.
 * 13) initialises the message manager.
 * 14) initialises the capability manager.
 * 15) initialise the scheduler manager.
 */

void			kernel_init(void)
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
   * 1)
   */

#ifdef CONF_STATS
  cons_msg('+', "starting stats manager\n");
#endif

  STATS_INIT();

  /*
   * 2)
   */

  cons_msg('+', "starting id manager\n");

  if (id_init() != ERROR_NONE)
    core_error("cannot initialise the id manager\n");

  /*
   * 3)
   */

  cons_msg('+', "starting set manager\n");

  if (set_init() != ERROR_NONE)
    core_error("cannot initialise the set manager\n");

  /*
   * 4)
   */

  cons_msg('+', "starting as manager\n");

  if (as_init() != ERROR_NONE)
    core_error("cannot initialise the address space manager\n");

  /*
   * 5)
   */

  cons_msg('+', "starting segment manager\n");

  if (segment_init() != ERROR_NONE)
    core_error("cannot initialise the segment manager\n");

  /*
   * 6)
   */

  cons_msg('+', "starting region manager\n");

  if (region_init(REGION_VMEM_MIN, REGION_VMEM_MAX - REGION_VMEM_MIN) !=
      ERROR_NONE)
    core_error("cannot initialise the region manager\n");

  /*
   * 7)
   */

  cons_msg('+', "starting map manager\n");

  if (map_init() != ERROR_NONE)
    core_error("cannot initialise the map manager\n");

  /*
   * 8)
   */

  cons_msg('+', "starting task manager\n");

  if (task_init() != ERROR_NONE)
    core_error("cannot initialise the task manager\n");

  /*
   * 9)
   */

  cons_msg('+', "starting thread manager\n");

  if (thread_init() != ERROR_NONE)
    core_error("cannot initialise the thread manager\n");

  /*
   * 10)
   */

  cons_msg('+', "starting event manager\n");

  if (event_init() != ERROR_NONE)
    core_error("cannot initialise the event manager\n");

  /*
   * 11)
   */

  cons_msg('+', "starting timer manager\n");

  if (timer_init() != ERROR_NONE)
    core_error("cannot initialise the timer manager\n");

  /* XXX remove me ! */

  //  event_test();

  //  timer_test();

  /*
   * 12)
   */

  cons_msg('+', "starting message manager\n");

  if (message_init() != ERROR_NONE)
    core_error("cannot initialise the message manager\n");

  /*
   * 13)
   */

  cons_msg('+', "starting capability manager\n");

  if (capability_init() != ERROR_NONE)
    core_error("cannot initialise the capability manager\n");

  /*
   * 14)
   */

  cons_msg('+', "starting scheduler manager\n");

  if (sched_init() != ERROR_NONE)
    core_error("cannot initialise the scheduler manager\n");
}

/*
 * this function cleans the kernel manager by cleaning every manager.
 */

void			kernel_clean(void)
{
  sched_clean();

  message_clean();

  timer_clean();

  event_clean();

  task_clean();

  map_clean();

  region_clean();

  segment_clean();

  as_clean();

  set_clean();

  id_clean();

  STATS_CLEAN();
}
