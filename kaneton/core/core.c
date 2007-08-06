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

#include <libc.h>
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
 * segment containing the mod service code.
 *
 * indeed, the kernel needs to retrieve this segment to map it and then
 * build a task for this very first service.
 */

i_segment		mod;

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
 * 6) performs the kernel work...
 * 7) cleans the kernel manager.
 * 8) shutdown the system.
 */

void			kaneton(t_init*				bootloader)
{
  /*
   * 1)
   */

#ifdef O2
  io_cons_init();

  printf("\n");
  printf("%s\n", version);
  printf("\n");
  mipsr10k_event_init();
  while(1);
#endif

  init = bootloader;

  /*
   * 2)
   */

  if (cons_init() != ERROR_NONE)
    core_error("cannot initialize the console manager\n");

  /*
   * 3)
   */

  printf("\n");
  cons_msg('+', "%s\n", version);
  printf("\n");

  /*
   * 4)
   */

  cons_msg('+', "starting malloc\n");

  alloc_init(init->alloc, init->allocsz);

  /*
   * 5)
   */

  cons_msg('+', "starting kernel manager\n");

  kernel_initialize();

#ifdef VIEW_ENABLE
  view_initialize();
#endif

  /*
   * 6)
   */

  cons_msg('+', "kaneton started\n");

  //  ibmpc_keyboard_init();

  STI();

#ifdef TESTSUITE_MANUAL_ENABLE
  cons_msg('+', "running manual tests\n");
  check_tests();
#endif

#if TESTSUITE_DEBUG_ENABLE
  cons_msg('+', "starting debug manager\n");
  debug_initialize();
#endif

  cons_msg('+', "launching the initial service: mod\n");
  if (kaneton_launch() != ERROR_NONE)
    cons_msg('!', "failed to start the initial mod service\n");

  while (1) /* XXX become IDLE */
    ;

  CLI();

  cons_msg('#', "kaneton is stopping...\n");

  /*
   * 7)
   */

#ifdef VIEW_ENABLE
  view_clean();
#endif

  kernel_clean();

  /*
   * 8)
   */

  cons_msg('+', "system shutdown\n");

  HLT();
}

/*
 * this function launches the very first service by creating a new
 * task.
 *
 * the new task's address space is only composed of a stack and the
 * code which is a mapping of the code provided by the booloader: init->mcode.
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
    i_task		mod;
  }			args;

  if (init->mcodesz == 0)
    {
      cons_msg('!', "no initial module\n");

      return (ERROR_UNKNOWN);
    }

  if (task_reserve(TASK_CLASS_PROGRAM,
		   TASK_BEHAV_TIMESHARING,
		   TASK_PRIOR_TIMESHARING,
		   &task) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (as_reserve(task, &as) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(as,
		     mod, 0,
		     REGION_OPT_FORCE,
		     init->mlocation, init->mcodesz,
		     &region) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_reserve(task, THREAD_PRIOR, &thread) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  stack.base = 0;
  stack.size = 4 * PAGESZ;

  if (thread_stack(thread, stack) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_get(thread, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = init->mentry;

  if (thread_load(thread, ctx) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  args.taskid = task;
  args.asid = as;
  args.mod = ID_UNUSED;

  if (thread_args(thread, &args, sizeof (args)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (task_state(task, SCHEDULER_STATE_RUN) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}
