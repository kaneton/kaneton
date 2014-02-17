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
 * updated       julien quintard   [sun jan 30 21:01:55 2011]
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

extern m_thread		_thread;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread dispatcher.
 */

d_thread		glue_thread_dispatch =
  {
    glue_thread_show,
    glue_thread_dump,
    glue_thread_reserve,
    glue_thread_release,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_thread_arguments,
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

t_status		glue_thread_show(i_thread		id,
					 mt_margin		margin)
{
  o_thread*		thread;
  as_context		ctx;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  /*
   * 2)
   */

  if (architecture_context_get(id, &ctx) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread's IA32 context");

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  machine: context(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      thread->machine.context);

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    stack: pointer(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      thread->machine.stack.pointer);

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    pile: base(0x%x) size(0x%x) pointer(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      thread->machine.pile.base,
	      thread->machine.pile.size,
	      thread->machine.pile.pointer);

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "    context: eax(0x%x) ebx(0x%x) ecx(0x%x) edx(0x%x) "
	      "esi(0x%x) edi(0x%x) ebp(0x%x) _esp(0x%x) esp(0x%x) eip(0x%x) "
	      "eflags(0x%x) cs(0x%x) ds(0x%x) ss(0x%x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      ctx.eax,
	      ctx.ebx,
	      ctx.ecx,
	      ctx.edx,
	      ctx.esi,
	      ctx.edi,
	      ctx.ebp,
	      ctx._esp,
	      ctx.esp,
	      ctx.eip,
	      ctx.eflags,
	      ctx.cs & 0xffff,
	      ctx.ds & 0xffff,
	      ctx.ss & 0xffff);

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

t_status		glue_thread_dump(void)
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
	      _thread.machine.selectors.kernel.cs,
	      _thread.machine.selectors.kernel.ds);

  module_call(console, message,
	      '#',
	      "      driver: CS(0x%x) DS(0x%x)\n",
	      _thread.machine.selectors.driver.cs,
	      _thread.machine.selectors.driver.ds);

  module_call(console, message,
	      '#',
	      "      service: CS(0x%x) DS(0x%x)\n",
	      _thread.machine.selectors.service.cs,
	      _thread.machine.selectors.service.ds);

  module_call(console, message,
	      '#',
	      "      guest: CS(0x%x) DS(0x%x)\n",
	      _thread.machine.selectors.guest.cs,
	      _thread.machine.selectors.guest.ds);

  MACHINE_LEAVE();
}

/*
 * this function reserves a thread by initializing its hardware context.
 *
 * steps:
 *
 * 1) initialize the thread's IA32 context.
 */

t_status		glue_thread_reserve(i_task		task,
					    t_priority		priority,
					    t_vaddr		stack,
					    t_vsize		stacksz,
					    t_vaddr		entry,
					    i_thread*		id)
{
  /*
   * 1)
   */

  if (architecture_context_build(*id) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the IA32 context");

  MACHINE_LEAVE();
}

/*
 * this function releases the thread's IA32-specific information.
 *
 * steps:
 *
 * 1) destory the IA32 context.
 */

t_status		glue_thread_release(i_thread		id)
{
  /*
   * 1)
   */

  if (architecture_context_destroy(id) != STATUS_OK)
    MACHINE_ESCAPE("unable to destory the IA32 context");

  MACHINE_LEAVE();
}

/*
 * this function loads the given context in the thread low-level structure.
 *
 * steps:
 *
 * 1) retrieve the thread's IA32 context.
 * 2) set some of the context's attributes: PC and SP.
 * 3) upadte the thread's IA32 context.
 */

t_status		glue_thread_load(i_thread		id,
					 s_thread_context	context)
{
  as_context		ctx;

  /*
   * 1)
   */

  if (architecture_context_get(id, &ctx) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread's IA32 context");

  /*
   * 2)
   */

  ctx.eip = context.pc;
  ctx.esp = context.sp;

  /*
   * 3)
   */

  if (architecture_context_set(id, &ctx) != STATUS_OK)
    MACHINE_ESCAPE("unable to update the IA32 context");

  MACHINE_LEAVE();
}

/*
 * this function returns the thread's current context.
 *
 * steps:
 *
 * 1) retrieve the thread's IA32 context.
 * 2) return the only core-meaningful values: PC and SP.
 */

t_status		glue_thread_store(i_thread		id,
					  s_thread_context*	context)
{
  as_context		ctx;

  /*
   * 1)
   */

  if (architecture_context_get(id, &ctx) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the IA32 context");

  /*
   * 2)
   */

  context->pc = ctx.eip;
  context->sp = ctx.esp;

  MACHINE_LEAVE();
}

/*
 * this function sets the given arguments on the thread's stack.
 */

t_status		glue_thread_arguments(i_thread		id,
					      void*		arguments,
					      t_vsize		size)
{
  if (architecture_context_arguments(id, arguments, size) != STATUS_OK)
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

t_status		glue_thread_initialize(void)
{
  /*
   * 1)
   */

  if (architecture_context_setup() != STATUS_OK)
    MACHINE_ESCAPE("unable to set up the context switcher");

  MACHINE_LEAVE();
}
