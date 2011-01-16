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
 * updated       julien quintard   [sat jan 15 15:43:44 2011]
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

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the thread manager.
 */

extern m_thread*	_thread;

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
 * this function shows a thread's machine-specific data.
 *
 * steps:
 *
 * 1) retrieve the thread object.
 * 2) retrieve the thread's IA32 context.
 * 3) display the thread's machine-specific information.
 */

t_error			glue_thread_show(i_thread		id,
					 mt_margin		margin)
{
  o_thread*		thread;
  t_ia32_context	context;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  /*
   * 2)
   */

  if (ia32_get_context(id, &context) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread's IA32 context");

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  machine: pile(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      thread->machine.pile);

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
 * this function dumps the thread manager's machine-specific information.
 *
 * steps:
 *
 * 1) display a general message.
 * 2) display the segment selectors.
 */

t_error			glue_thread_dump(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '#',
	      "  machine:\n");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      "    selectors:\n");

  module_call(console, message,
	      '#',
	      "      kernel: CS(0x%x) DS(0x%x)\n",
	      _thread->machine.selectors.kernel.cs,
	      _thread->machine.selectors.kernel.ds);

  module_call(console, message,
	      '#',
	      "      driver: CS(0x%x) DS(0x%x)\n",
	      _thread->machine.selectors.driver.cs,
	      _thread->machine.selectors.driver.ds);

  module_call(console, message,
	      '#',
	      "      service: CS(0x%x) DS(0x%x)\n",
	      _thread->machine.selectors.service.cs,
	      _thread->machine.selectors.service.ds);

  module_call(console, message,
	      '#',
	      "      guest: CS(0x%x) DS(0x%x)\n",
	      _thread->machine.selectors.guest.cs,
	      _thread->machine.selectors.guest.ds);

  MACHINE_LEAVE();
}

/*
 * this function reserves a thread by initializing its hardware context.
 *
 * steps:
 *
 * 1) initialize the thread's IA32 context.
 */

t_error			glue_thread_reserve(i_task		taskid,
					    i_thread*		threadid)
{
  /*
   * 1)
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
  if (ia32_status_context(threadid, &context->pc, &context->sp) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the IA32 context");

  // XXX ici on devrai recup le ia32 context puis dans cette fonction
  // faire le trie et remplir pc et sp plutot que d'avoir une fonction
  // arch specifique.
  // XXX meme chose pour load()
  // XXX update steps

  MACHINE_LEAVE();
}

/*
 * this function sets the given arguments on the thread's stack.
 */

t_error			glue_thread_args(i_thread		threadid,
					 const void*	       	args,
					 t_vsize		size)
{
  if (ia32_push_args(threadid, args, size) != ERROR_OK)
    MACHINE_ESCAPE("unable to push the arguments");

  MACHINE_LEAVE();
}

/*
 * this function initializes the thread manager's glue.
 *
 * steps:
 *
 * 1) initialize the context switcher.
 */

t_error			glue_thread_initialize(void)
{
  /*
   * 1)
   */

  if (ia32_init_switcher() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the context switcher");

  MACHINE_LEAVE();
}
