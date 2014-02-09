/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/core.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sat apr 23 12:34:20 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file is the entry point of the kaneton microkernel.
 *
 * from the kaneton() function, the kernel manager will be initialized
 * and then cleaned.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * this variable is generated when the kernel is compiled, indicating
 * the current kernel version.
 */

extern const char	version[];

/*
 * init variable received from the bootloader specifying segments, regions,
 * physical memory layout etc.
 */

s_init*			_init;

/*
 * this variable is filled by the task manager when it injects the
 * pre-reserved segments. this variable holds the identifier of the
 * segment containing the system service code.
 *
 * indeed, the kernel needs to retrieve this segment to map it and then
 * build a task for this very first service.
 */

i_segment		_system;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function simply initializes and cleans the kernel manager.
 *
 * steps:
 *
 * 1) set the _init variable from the boot loader argument.
 * 2) load some fundamental modules.
 * 3) displays the current kaneton version.
 * 4) set up the fine grained allocator.
 * 5) start the kernel.
 * 6) run the test system, should have it be loaded.
 * 7) spawn the 'system' service.
 * 8) start the scheduler.
 * 9) start the event processing, action which will have the effet of
 *    activating the hardware interrupts, hence trigger context switchs. then,
 *    should the execution come back to the kernel, its first task is to
 *    actually disable the events so that the execution does not go away.
 * 10) stop the kernel.
 * 11) unload the modules.
 * 12) shutdown the system.
 */

void			kaneton(s_init*				init)
{
  i_cpu			cpu;

  /*
   * 1)
   */

  _init = init;

  /*
   * 2)
   */

  module_load(console);
  module_load(report);
  module_load(forward);
  module_load(test);
  module_load(debug);

  /*
   * 3)
   */

  module_call(console, message,
	      '+', "%s\n", version);

  /*
   * 4)
   */

  module_call(console, message,
	      '+', "setting up the fine-graind memeory allocator\n");

  alloc_init(_init->alloc, _init->allocsz);

  /*
   * 5)
   */

  module_call(console, message,
	      '+', "starting the kernel\n");

  assert(kernel_initialize() == STATUS_OK);

  /*
   *
   */
  module_call(debug, start);

  /*
   * 6)
   */

  module_call(test, run);

  /*
   * 7)
   */

  module_call(console, message,
	      '+', "spawning the 'system' server\n");

  assert(kaneton_spawn() == STATUS_OK);

  /*
   * 8)
   */

  assert(cpu_current(&cpu) == STATUS_OK);

  assert(scheduler_start(cpu) == STATUS_OK);

  /*
   * 9)
   */

  assert(event_enable() == STATUS_OK);
  assert(event_disable() == STATUS_OK);

  /*
   * 10)
   */

  module_call(console, message,
	      '+', "stopping the kernel\n");

  assert(kernel_clean() == STATUS_OK);

  /*
   * 11)
   */

  module_unload(debug);
  module_unload(test);
  module_unload(forward);
  module_unload(report);
  module_unload(console);

  /*
   * 12)
   */

  while (1)
    ;
}

/*
 * this function launches the very first server by creating a new
 * task.
 *
 * the new task's address space is only composed of a stack and the
 * code which is a mapping of the code provided by the booloader:
 * init->scode.
 *
 * this first server is referred to as the 'system' server.
 *
 * steps:
 *
 * 1) check that the boot loader provided a system to spawn through
 *    the _slocation_ and _scodesz_ attributes of the init structure.
 * 2) reserve a task and addresse space for the server.
 * 3) map the server's code segment, i.e _system (c.f. initialize() in task.c)
 *    at a very precise location.
 * 4) map the array of inputs (servers to be launch at boot time by the
 *    'system' service) so that the server can access and spawn them. note
 *    that the inputs are mapped so that the physical and virtual addresses
 *    match, method referred to as identity mapping.
 * 5) reserve a thread.
 * 6) builds an arguments array containing the address of the inputs array.
 *    place these arguments so that the thread can retrieve them in the
 *    main() function.
 * 7) set the task as running.
 * 8) start the thread.
 */

t_status		kaneton_spawn(void)
{
  i_thread		thread;
  i_region		region;
  i_segment		segment;
  i_task		task;
  i_as			as;
  struct
  {
    i_task		task;
    i_as		as;
    int			argc;
    char*		argv;
    char*		envp;
  }			arguments;

  /*
   * 1)
   */

  if (_init->scodesz == 0)
    CORE_ESCAPE("there is no 'system' server to launch");

  /*
   * 2)
   */

  if (task_reserve(TASK_CLASS_SERVICE,
		   TASK_BEHAVIOUR_TIMESHARING,
		   TASK_PRIORITY_TIMESHARING,
		   &task) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a task");

  if (as_reserve(task, &as) != STATUS_OK)
    CORE_ESCAPE("unable to reserve an address space");

  /*
   * 3)
   */

  if (region_reserve(as,
		     _system,
		     0,
		     REGION_OPTION_FORCE,
		     _init->slocation,
		     _init->scodesz,
		     &region) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the region for the server code");

  /*
   * 4)
   */

  if (segment_locate((t_paddr)_init->inputs, &segment) == FALSE)
    CORE_ESCAPE("unable to locate the segment holding the given address");

  if (region_reserve(as,
		     segment,
		     0,
		     REGION_OPTION_FORCE,
		     (t_vaddr)_init->inputs,
		     _init->inputssz,
		     &region) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the region for the inputs");

  /*
   * 5)
   */

  if (thread_reserve(task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
		     THREAD_STACK_SIZE,
		     _init->sentry,
		     &thread) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a thread");

  /*
   * 6)
   */

  arguments.task = task;
  arguments.as = as;
  arguments.argc = 1;
  arguments.argv = (char*)_init->inputs;
  arguments.envp = NULL;

  if (thread_arguments(thread, &arguments, sizeof (arguments)) != STATUS_OK)
    CORE_ESCAPE("unable to pass arguments to the thread");

  /*
   * 7)
   */

  if (task_start(task) != STATUS_OK)
    CORE_ESCAPE("unable to start the task");

  /*
   * 8)
   */

  if (thread_start(thread) != STATUS_OK)
    CORE_ESCAPE("unable to start the thread");

  CORE_LEAVE();
}
