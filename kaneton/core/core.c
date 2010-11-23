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

t_init*			_init;

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
 * 1) sets the init variable from the bootloader argument.
 * 2) initializes some fundamental modules.
 * 3) displays the current kaneton version.
 * 4) initializes the fine grained allocator.
 * 5) initializes the report manager.
 * 6) initializes the kernel manager.
 * 7) initializes the test system.
 * 8) launches the very first service.
 * 9) cleans the kernel manager.
 * 10) shutdown the system.
 */

void			kaneton(t_init*				bootloader)
{
  /*
   * 1)
   */

  _init = bootloader;

  /*
   * 2)
   */

  module_call(console, console_initialize);
  module_call(forward, forward_initialize);

  /*
   * 3)
   */

  module_call(console, console_message,
	      '+', "%s\n", version);

  /*
   * 4)
   */

  module_call(console, console_message,
	      '+', "setting up the fine-graind memeory allocator\n");

  alloc_init(_init->alloc, _init->allocsz);

  /*
   * 5)
   */

  module_call(report, report_initialize);

  /*
   * 6)
   */

  module_call(console, console_message,
	      '+', "starting the kernel\n");

  kernel_initialize();

  /*
   * 7)
   */

  module_call(test, test_run);

  /*
   * 8)
   */

  module_call(console, console_message,
	      '+', "spawning the 'system' server\n");

  kaneton_spawn();

  /*
   * 9)
   */

  module_call(console, console_message,
	      '+', "stopping the kernel\n");

  kernel_clean();

  /*
   * 10)
   */

  while (1)
    ;
}

/*
 * this function launches the very first service by creating a new
 * task.
 *
 * the new task's address space is only composed of a stack and the
 * code which is a mapping of the code provided by the booloader: init->scode.
 */

t_error			kaneton_spawn(void)
{
  i_thread		thread;
  i_region		region;
  t_stack		stack;
  i_task		task;
  t_thread_context	ctx;
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

  assert(_init->scodesz != 0);

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_TIMESHARING,
		   TASK_PRIORITY_TIMESHARING,
		   &task) != ERROR_OK)
    return (ERROR_KO);

  if (as_reserve(task, &as) != ERROR_OK)
    return (ERROR_KO);

  if (region_reserve(as,
		     _system,
		     0,
		     REGION_OPTION_FORCE,
		     _init->slocation,
		     _init->scodesz,
		     &region) != ERROR_OK)
    return (ERROR_KO);

  if (region_reserve(as,
		     (i_segment)(t_vaddr)_init->inputs,
		     0,
		     REGION_OPTION_FORCE,
		     (t_vaddr)_init->inputs,
		     _init->inputssz,
		     &region) != ERROR_OK)
    return (ERROR_KO);

  if (thread_reserve(task, THREAD_PRIORITY, &thread) != ERROR_OK)
    return (ERROR_KO);

  stack.base = 0;
  stack.size = 10 * PAGESZ;

  if (thread_stack(thread, stack) != ERROR_OK)
    return (ERROR_KO);

  if (thread_get(thread, &o) != ERROR_OK)
    return (ERROR_KO);

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = _init->sentry;

  if (thread_load(thread, ctx) != ERROR_OK)
    return (ERROR_KO);

  args.task = task;
  args.as = as;
  args.argc = 1;
  args.argv = (char*)_init->inputs;
  args.envp = NULL;

  if (thread_args(thread, &args, sizeof (args)) != ERROR_OK)
    return (ERROR_KO);

  if (task_state(task, TASK_STATE_RUN) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
}
