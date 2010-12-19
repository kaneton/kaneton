/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/glue/ibm-pc.ia32/educational/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       julien quintard   [sun dec 19 18:29:15 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the thread manager's glue.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread dispatcher.
 */

d_thread		glue_thread_dispatch =
  {
    glue_thread_show,
    NULL,
    glue_thread_reserve,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_thread_args,
    NULL,
    NULL,
    glue_thread_load,
    glue_thread_store,
    glue_thread_initialize,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX
 */

t_error			glue_thread_show(i_thread		id,
					 mt_margin		margin)
{
  o_thread*		thread;
  t_ia32_context	context;

  if (thread_get(id, &thread) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  machine: pile(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      thread->machine.pile);

  if (ia32_get_context(id, &context) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread's IA32 context");

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    context: eax(0x%x) ebx(0x%x) ecx(0x%x) edx(0x%x) "
	      "esi(0x%x) edi(0x%x) ebp(0x%x) esp(0x%x) eip(0x%x) "
	      "eflags(0x%x) cs(0x%x) ds(0x%x) ss(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      context.eax,
	      context.ebx,
	      context.ecx,
	      context.edx,
	      context.esi,
	      context.edi,
	      context.ebp,
	      context.esp,
	      context.eip,
	      context.eflags,
	      context.cs,
	      context.ds,
	      context.ss);

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_thread_dump(void)
{
  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  machine:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    selectors:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "      kernel: cs(0x%x) ds(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      _thread->machine.selectors.kernel.cs,
	      _thread->machine.selectors.kernel.ds);

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "      driver: cs(0x%x) ds(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      _thread->machine.selectors.driver.cs,
	      _thread->machine.selectors.driver.ds);

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "      service: cs(0x%x) ds(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      _thread->machine.selectors.service.cs,
	      _thread->machine.selectors.service.ds);

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "      guest: cs(0x%x) ds(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      _thread->machine.selectors.guest.cs,
	      _thread->machine.selectors.guest.ds);

  MACHINE_LEAVE();
}

/*
 * this function reserves a thread by initializing its hardware context.
 */

t_error			glue_thread_reserve(i_task		taskid,
					    i_thread*		threadid)
{
  /*
   * XXX
   */

  if (ia32_init_context(taskid, *threadid) != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the IA32 context");

  MACHINE_LEAVE();
}

/*
 * this function loads the given context in the thread low-level structure.
 */

t_error			glue_thread_load(i_thread		threadid,
					 s_thread_context	context)
{
  /*
   * XXX
   */

  if (ia32_setup_context(threadid, context.pc, context.sp) != ERROR_OK)
    MACHINE_ESCAPE("unable to set up the IA32 context");

  MACHINE_LEAVE();
}

/*
 * this function returns the thread's current context.
 */

t_error			glue_thread_store(i_thread		threadid,
					  s_thread_context*	context)
{
  /*
   * XXX
   */

  if (ia32_status_context(threadid, &context->pc, &context->sp) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the IA32 context");

  // XXX ici on devrai recup le ia32 context et copier ce qu'il faut dans le
  // thread context.

  MACHINE_LEAVE();
}

/*
 * this function sets the given arguments on the thread's stack.
 */

t_error			glue_thread_args(i_thread		threadid,
					 const void*	       	args,
					 t_vsize		size)
{
  /*
   * XXX
   */

  if (ia32_push_args(threadid, args, size) != ERROR_OK)
    MACHINE_ESCAPE("unable to push the arguments");

  MACHINE_LEAVE();
}

/*
 * this function initializes the thread manager's glue.
 */

t_error			glue_thread_initialize(void)
{
  /*
   * XXX
   */

  // XXX mieux dans scheduler: ici il y avait ia32_init_switcher()
  // XXX finalement on le laisse ici car thread_load() en a besoin
  // et une tache pourrait etre reservee avant que le sched ne soit init.
  if (ia32_init_switcher() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the context switcher");

  MACHINE_LEAVE();
}
