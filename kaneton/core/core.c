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

t_init*			init;

/*
 * this variable is filled by the task manager when it injects the
 * pre-reserved segments. this variable holds the identifier of the
 * segment containing the system service code.
 *
 * indeed, the kernel needs to retrieve this segment to map it and then
 * build a task for this very first service.
 */

i_segment		system;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function simply initializes and cleans the kernel manager.
 *
 * steps:
 *
 * 1) sets the init variable from the bootloader argument.
 * 2) initializes the console manager.
 * 3) displays the current kaneton version.
 * 4) initializes the fine grained allocator.
 * 5) initializes the kernel manager.
 * 6) initializes the test system.
 * 7) launches the very first service.
 * 8) cleans the kernel manager.
 * 9) shutdown the system.
 */

void			kaneton(t_init*				bootloader)
{

#ifdef MIPS64_DEV
  while(1);
#endif

#ifdef O2
  io_cons_init();

  printf("\n");
  printf("%s\n", version);
  printf("\n");
  mipsr10k_event_init();
  while(1);
#endif


  /*
   * 1)
   */

  init = bootloader;

  /*
   * 2)
   */

  module_call(console, console_initialize);

  /*
   * 3)
   */

  printf("\n");
  module_call(console, console_message, '+', "%s\n", version);
  printf("\n");

  /*
   * 4)
   */

  module_call(console, console_message, '+', "starting malloc\n");

  alloc_init(init->alloc, init->allocsz);

  /*
   * 5)
   */

  module_call(console, console_message, '+', "starting kernel manager\n");

  kernel_initialize();

  module_call(console, console_message, '+', "kaneton started\n");

  /*
   * 6)
   */

  module_call(test, test_run);

  /*
   * 7)
   */

  module_call(console, console_message, '+', "launching the initial server\n");

  if (kaneton_launch() != ERROR_NONE)
    module_call(console, console_message, '!', "failed to start the initial server\n");

#ifdef IA32_DEPENDENT
  STI();
#endif

  // XXX this should never be reached. since a least a task
  // has been created, syscalls are made!
  char c = '-';

  while (1) /* XXX become IDLE */
    {
      if (c == '-')
	c = '\\';
      else if (c == '\\')
	c = '|';
      else if (c == '|')
	c = '/';
      else
	c = '-';

      printf("\r %c", c);
    }

#ifdef IA32_DEPENDENT
  CLI();
#endif

  module_call(console, console_message, '#', "kaneton is stopping...\n");

  /*
   * 7)
   */

  kernel_clean();

  /*
   * 8)
   */

  module_call(console, console_message, '+', "system shutdown\n");

#ifdef IA32_DEPENDENT
  HLT();
#endif
}

/*
 * this function launches the very first service by creating a new
 * task.
 *
 * the new task's address space is only composed of a stack and the
 * code which is a mapping of the code provided by the booloader: init->scode.
 */

t_error			kaneton_launch(void)
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
    i_task		taskid;
    i_as		asid;
    int			argc;
    char*		argv;
    char*		envp;
  }			args;

  if (init->scodesz == 0)
    {
      module_call(console, console_message, '!', "no initial server provided in the inputs\n");

      return (ERROR_UNKNOWN);
    }

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_TIMESHARING,
		   TASK_PRIOR_TIMESHARING,
		   &task) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (as_reserve(task, &as) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(as,
		     system, 0,
		     REGION_OPT_FORCE,
		     init->slocation, init->scodesz,
		     &region) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(as,
		     (i_segment)(t_vaddr)init->inputs, 0,
		     REGION_OPT_FORCE,
		     (t_vaddr)init->inputs, init->inputssz,
		     &region) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_reserve(task, THREAD_PRIOR, &thread) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  stack.base = 0;
  stack.size = 10 * PAGESZ;

  if (thread_stack(thread, stack) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_get(thread, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = init->sentry;

  if (thread_load(thread, ctx) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  args.taskid = task;
  args.asid = as;
  args.argc = 1;
  args.argv = (char*)init->inputs;
  args.envp = NULL;

  if (thread_args(thread, &args, sizeof (args)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (task_state(task, SCHEDULER_STATE_RUN) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

