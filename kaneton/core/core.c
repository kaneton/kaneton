/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/core.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sun may  6 18:44:10 2007]
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

  assert(kernel_initialize() == ERROR_OK);

  // XXX
  {
    as_dump();
    cpu_dump();
    event_dump();
    extern m_as* _as;
    id_show(&_as->id, 0);
    kernel_dump();
    s_clock clock;
    clock_current(&clock);
    clock_show(&clock, 0);
    module_call(console, print,
		"[kaneton] %u [page size] %u [endian] %u "
		"[wordsz] %u [kernel] %u\n",
		___kaneton,
		___kaneton$pagesz, ___kaneton$endian,
		___kaneton$wordsz, ___kaneton$kernel);
    region_dump(0);
    scheduler_dump();
    segment_dump();
    set_dump();
  }
  // XXX

  /*
   * 6)
   */

  module_call(test, run);

  /*
   * 7)
   */

  module_call(console, message,
	      '+', "spawning the 'system' server\n");

  assert(kaneton_spawn() == ERROR_OK);

  /*
   * 8)
   */

  assert(scheduler_current(&cpu) == ERROR_OK);

  assert(scheduler_start(cpu) == ERROR_OK);

  /*
   * 9)
   */

  assert(event_enable() == ERROR_OK);
  assert(event_disable() == ERROR_OK);

  /*
   * 10)
   */

  module_call(console, message,
	      '+', "stopping the kernel\n");

  assert(kernel_clean() == ERROR_OK);

  /*
   * 11)
   */

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
 *    'system' service) so that the server can access and spawn them.
 * 5) reserve a thread.
 * 6) set up the thread stack.
 * 7) initialize the thread context, especially the entry point.
 * 8) builds an arguments array containing the address of the inputs array.
 *    place these arguments so that the thread can retrieve them in the
 *    main() function.
 * 9) set the task as running.
 * 10) start the thread.
 */

t_error			kaneton_spawn(void)
{
  i_thread		thread;
  i_region		region;
  s_stack		stack;
  i_task		task;
  s_thread_context	ctx;
  i_as			as;
  o_thread*		o;
  struct
  {
    i_task		task;
    i_as		as;
    int			argc;
    char*		argv;
    char*		envp;
  }			args;

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
		   &task) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a task");

  if (as_reserve(task, &as) != ERROR_OK)
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
		     &region) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the region for the server code");

  /*
   * 4)
   */

  if (region_reserve(as,
		     (i_segment)(t_vaddr)_init->inputs,
		     0,
		     REGION_OPTION_FORCE,
		     (t_vaddr)_init->inputs,
		     _init->inputssz,
		     &region) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the region for the inputs");

  /*
   * 5)
   */

  if (thread_reserve(task, THREAD_PRIORITY, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a thread");

  /*
   * 6)
   */

  stack.base = 0;
  stack.size = THREAD_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_OK)
    CORE_ESCAPE("unable to set up the thread's stack");

  /*
   * 7)
   */

  if (thread_get(thread, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = _init->sentry;

  if (thread_load(thread, ctx) != ERROR_OK)
    CORE_ESCAPE("unable to load the initial thread's context");

  /*
   * 8)
   */

  args.task = task;
  args.as = as;
  args.argc = 1;
  args.argv = (char*)_init->inputs;
  args.envp = NULL;

  if (thread_args(thread, &args, sizeof (args)) != ERROR_OK)
    CORE_ESCAPE("unable to pass arguments to the thread");

  /*
   * 9)
   */

  if (task_start(task) != ERROR_OK)
    CORE_ESCAPE("unable to start the task");

  /*
   * 10)
   */

  if (thread_start(thread) != ERROR_OK)
    CORE_ESCAPE("unable to start the thread");

  CORE_LEAVE();
}
