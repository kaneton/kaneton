/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/kernel/kernel.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [thu apr  7 14:23:31 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains the kernel manager's code.
 *
 * this manager, unlike the others, does not manage a specific kaneton
 * object but rather the whole kernel. indeed, this manager does not provide
 * any functionality but to initialize and clean the kernel by calling
 * all the other managers.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(kernel);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the init structure.
 */

extern s_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

m_kernel		_kernel;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the kernel manager.
 *
 * steps:
 *
 * 1) build the endian string.
 * 2) display information.
 * 3) call the machine.
 */

t_status		kernel_dump(void)
{
  char*			endian;

  /*
   * 1)
   */

  switch (___kaneton$endian)
    {
    case ENDIAN_LITTLE:
      {
	endian = "little";

	break;
      }
    case ENDIAN_BIG:
      {
	endian = "big";

	break;
      }
    default:
      CORE_ESCAPE("unknown endianness '%u'",
		  ___kaneton$endian);
    }

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      "kernel manager:\n");

  module_call(console, message,
	      '#',
	      "  core: cell(%qd) task(%qd) as(%qd) thread(%qd) "
	      "node(%qd:%qd) endian(%s) wordsz(%u) framesz(%u) pagesz(%u)\n",
	      _kernel.cell,
	      _kernel.task, _kernel.as, _kernel.thread,
	      _kernel.node.cell, _kernel.node.task,
	      endian,
	      ___kaneton$wordsz, ___kaneton$framesz, ___kaneton$pagesz);

  /*
   * 3)
   */

  if (machine_call(kernel, dump) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function initializes the kernel manager by initializing every
 * other manager.
 *
 * note that the kernel manager's components such as the task, thread,
 * address space etc. are not reserved where one may expect to. for instance,
 * the kernel task is reserve in task_initialize() along with the address
 * space. however the kernel thread is reserved in thread_initialize().
 *
 * in addition, the task_initialize() function injects the pre-reserved
 * segments and regions passed by the boot loaded through the 'init'
 * structure.
 *
 * steps:
 *
 * 1) initialize the kernel structure but also set the kernel
 *    identifiers---task, as and thread---as being not initialised.
 * 2) initialize the identifier manager.
 * 3) initialize the set manager.
 * 4) initialize the address space manager.
 * 5) initialize the segment manager.
 * 6) initialize the region manager.
 * 7) initialize the map manager
 * 8) initialize the CPU manager.
 * 9) initialize the task manager.
 * 10) initialize the thread manager.
 * 11) now that _everything_ related to memory manager is ready, set up
 *     the fine grain memory allocator in order to work with the kernel
 *     managers rather than the pre-allocated memory area provided by
 *     the boot loader.
 *     note that the task manager must have been initialized since it is
 *     its reponsability to reserve the kernel task, thread and address
 *     space and then to inject the init's segments and regions in the
 *     kernel's address space.
 * 12) initialize the event manager.
 * 13) initialize the timer manager.
 * 14) initialize the clock manager.
 * 15) initialize the I/O manager.
 * 16) initialize the scheduler manager.
 * 17) generate the random kaneton cell identifier and sets up the
 *     kaneton node identifier.
 * 18) initialize the capability manager.
 * 19) initialize the message manager.
 * 20) call the machine.
 */

t_status		kernel_initialize(void)
{
  /*
   * 1)
   */

  memset(&_kernel, 0x0, sizeof (m_kernel));

  _kernel.task = ID_UNUSED;
  _kernel.as = ID_UNUSED;
  _kernel.thread = ID_UNUSED;

  /*
   * 2)
   */

  if (id_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the identifier manager");

  /*
   * 3)
   */

  if (set_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the set manager");

  /*
   * 4)
   */

  if (as_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the address space manager");

  /*
   * 5)
   */

  if (segment_initialize(_init->pbase,
			 _init->psize) != STATUS_OK)
    CORE_ESCAPE("unable to initialize the segment manager");

  /*
   * 6)
   */

  if (region_initialize(_init->vbase,
			_init->vsize) != STATUS_OK)
    CORE_ESCAPE("unable to initialize the region manager");

  /*
   * 7)
   */

  if (map_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the map manager");

  /*
   * 8)
   */

  if (cpu_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the CPU manager");

  /*
   * 9)
   */

  if (task_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the task manager");

  /*
   * 10)
   */

  if (thread_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the thread manager");

  /*
   * 11)
   */

  alloc_setup();

  /*
   * 12)
   */

  if (event_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the event manager");

  /*
   * 13)
   */

  if (timer_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the timer manager");

  /*
   * 14)
   */

  if (clock_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the clock manager");

  /*
   * 15)
   */

  if (io_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the I/O manager");

  /*
   * 16)
   */

  if (scheduler_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the scheduler manager");

  /*
   * 17)
   */

  random_seed();

  _kernel.cell = random_generate();

  _kernel.node.cell = _kernel.cell;
  _kernel.node.task = _kernel.task;

  /*
   * 18)
   */

  if (capability_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the capability manager");

  /*
   * 19)
   */

  if (message_initialize() != STATUS_OK)
    CORE_ESCAPE("unable to initialize the message manager");

  /*
   * 20)
   */

  if (machine_call(kernel, initialize) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the kernel manager by cleaning every manager.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) clean the message manager.
 * 3) clean the capability manager.
 * 4) clean the scheduler manager.
 * 5) clean the I/O manager.
 * 6) clean the clock manager.
 * 7) clean the timer manager.
 * 8) clean the event manager.
 * 9) clean the thread manager.
 * 10) clean the task manager.
 * 11) clean the CPU manager.
 * 12) clean the map manager.
 * 13) clean the region manager.
 * 14) clean the segment manager.
 * 15) clean the address space manager.
 * 16) clean the set manager.
 * 17) clean the identifier manager.
 */

t_status		kernel_clean(void)
{
  /*
   * 1)
   */

  if (machine_call(kernel, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (message_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the message manager");

  /*
   * 3)
   */

  if (capability_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the capability manager");

  /*
   * 4)
   */

  if (scheduler_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the scheduler manager");

  /*
   * 5)
   */

  if (io_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the I/O manager");

  /*
   * 6)
   */

  if (clock_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the clock manager");

  /*
   * 7)
   */

  if (timer_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the timer manager");

  /*
   * 8)
   */

  if (event_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the event manager");

  /*
   * 9)
   */

  if (thread_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the thread manager");

  /*
   * 10)
   */

  if (task_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the task manager");

  /*
   * 11)
   */

  if (cpu_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the CPU manager");

  /*
   * 12)
   */

  if (map_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the map manager");

  /*
   * 13)
   */

  if (region_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the region manager");

  /*
   * 14)
   */

  if (segment_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the segment manager");

  /*
   * 15)
   */

  if (as_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the address space manager");

  /*
   * 16)
   */

  if (set_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the set manager");

  /*
   * 17)
   */

  if (id_clean() != STATUS_OK)
    CORE_ESCAPE("unable to clean the identifier manager");

  CORE_LEAVE();
}
