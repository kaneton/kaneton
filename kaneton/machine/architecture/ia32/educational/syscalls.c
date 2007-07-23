/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/architecture/ia32/educational/syscalls.c
 *
 * created       matthieu bucchianeri   [sat jun 17 19:38:22 2006]
 * updated       matthieu bucchianeri   [fri jun 15 09:17:19 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file declares and implements the primitive system calls.
 *
 * syscalls map is:
 *
 *  + 56: message_register
 *  + 57: message_send
 *  + 58: message_transmit
 *  + 59: message_throw
 *  + 60: message_receive
 *  + 61: message_grab
 *  + 62: message_poll
 *  + 63: message_state
 *  + 64: message_wait
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the message manager global.
 */

extern m_message*	message;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k4 */

/*
 * handler for message_register syscall. prerequisites: message_register never
 * blocks.
 */

void			ia32_syshandler_register(void)
{
  t_error		res;
  i_thread		caller;
  i_task		task;
  t_type		type;
  t_vsize		size;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);

  ASSERT(task_current(&task) == ERROR_NONE);

  type = ia32_context->eax;
  size = ia32_context->ebx;

  res = message_register(task, type, size);

  ia32_context->eax = res;
}

/*
 * handler for message_send syscall. prerequisites: message_send never blocks.
 */

void			ia32_syshandler_send(void)
{
  t_error		res;
  i_thread		caller;
  i_task		task;
  t_type		type;
  t_vaddr		data;
  t_vsize		size;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);

  ASSERT(task_current(&task) == ERROR_NONE);

  u.reg[0] = ia32_context->eax;
  u.reg[1] = ia32_context->ebx;
  u.reg[2] = ia32_context->ecx;
  u.reg[3] = ia32_context->edx;
  type = ia32_context->esi;
  data = ia32_context->edi;
  size = ia32_context->ebp;

  res = message_send(task, u.node, type, data, size);

  ia32_context->eax = res;
}

/*
 * handler for message_transmit syscall. message_transmit can block.
 */

void			ia32_syshandler_transmit(void)
{
  t_error		res;
  i_thread		caller;
  i_thread		current;
  i_task		task;
  t_type		type;
  t_vaddr		data;
  t_vsize		size;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);

  ASSERT(task_current(&task) == ERROR_NONE);

  u.reg[0] = ia32_context->eax;
  u.reg[1] = ia32_context->ebx;
  u.reg[2] = ia32_context->ecx;
  u.reg[3] = ia32_context->edx;
  type = ia32_context->esi;
  data = ia32_context->edi;
  size = ia32_context->ebp;

  res = message_transmit(task, u.node, type, data, size);

  ASSERT(scheduler_current(&current) == ERROR_NONE);

  if (current == caller)
    ia32_context->eax = res;
}

/*
 * handler for message_receive syscall. message_receive can block.
 */

void			ia32_syshandler_receive(void)
{
  t_error		res;
  i_thread		caller;
  i_thread		current;
  i_task		task;
  t_type		type;
  t_vaddr		data;
  t_vsize		size;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);

  ASSERT(task_current(&task) == ERROR_NONE);

  type = ia32_context->eax;
  data = ia32_context->ebx;

  res = message_receive(task, type, data, &size, &u.node);

  ASSERT(scheduler_current(&current) == ERROR_NONE);

  if (current == caller)
    {
      ia32_context->eax = res;
      ia32_context->ebx = size;
      ia32_context->ecx = u.reg[0];
      ia32_context->edx = u.reg[1];
      ia32_context->esi = u.reg[2];
      ia32_context->edi = u.reg[3];
    }
}

/*
 * handler for message_poll syscall. prerequisites: message_poll neven blocks.
 */

void			ia32_syshandler_poll(void)
{
  t_error		res;
  i_thread		caller;
  i_task		task;
  t_type		type;
  t_vaddr		data;
  t_vsize		size;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);

  ASSERT(task_current(&task) == ERROR_NONE);

  type = ia32_context->eax;
  data = ia32_context->ebx;

  res = message_poll(task, type, data, &size, &u.node);

  ia32_context->eax = res;
  ia32_context->ebx = size;
  ia32_context->ecx = u.reg[0];
  ia32_context->edx = u.reg[1];
  ia32_context->esi = u.reg[2];
  ia32_context->edi = u.reg[3];
}

/*
 * this function set the return code of a blocked syscall (message_transmit).
 * the thread must not be executing.
 */

t_error			ia32_syscall_set_code(i_thread		thread,
					      t_error		error)
{
  o_thread*		o;

  if (thread_get(thread, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  o->machdep.context.eax = error;

  return (ERROR_NONE);
}

/*
 * this function set the returned info of a blocked syscall (message_receive).
 * the thread must not be executing.
 */

t_error			ia32_syscall_set_info(i_thread		thread,
					      t_error		error,
					      t_vsize		size,
					      i_node		sender)
{
  o_thread*		o;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  if (thread_get(thread, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  u.node = sender;

  o->machdep.context.eax = error;
  o->machdep.context.ebx = size;
  o->machdep.context.ecx = u.reg[0];
  o->machdep.context.edx = u.reg[1];
  o->machdep.context.esi = u.reg[2];
  o->machdep.context.edi = u.reg[3];

  return (ERROR_NONE);
}

/*
 * this function registers the different syscalls.
 */

t_error			ia32_syscalls_init(void)
{
  if (event_reserve(56, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_register)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (event_reserve(57, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_send)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (event_reserve(58, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_transmit)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /* XXX 59 */

  if (event_reserve(60, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_receive)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /* XXX 61 */

  if (event_reserve(62, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_poll)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /* XXX 63 */
  /* XXX 64 */

  return (ERROR_NONE);
}

/*
 * this function unregisters the different syscalls.
 */

t_error			ia32_syscalls_clean(void)
{
  if (event_release(56) != ERROR_NONE ||
      event_release(57) != ERROR_NONE ||
      event_release(58) != ERROR_NONE ||
      event_release(59) != ERROR_NONE ||
      event_release(60) != ERROR_NONE ||
      event_release(61) != ERROR_NONE ||
      event_release(62) != ERROR_NONE ||
      event_release(63) != ERROR_NONE ||
      event_release(64) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k4 */
