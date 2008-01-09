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
 *  + 65: message_size
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc/libc.h>
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

/* [cut k4] */

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
  t_ia32_context	ctx;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);
  ASSERT(task_current(&task) == ERROR_NONE);

  ASSERT(ia32_get_context(caller, &ctx));

  type = ctx.eax;
  size = ctx.ebx;

  res = message_register(task, type, size);

  ctx.eax = res;

  ASSERT(ia32_set_context(caller, &ctx, IA32_CONTEXT_EAX));
}


/*
 * handler for message_size syscall. prerequisites: message_size never
 * blocks.
 */

void			ia32_syshandler_size(void)
{
  t_error		res;
  i_thread		caller;
  i_task		task;
  t_type		type;
  t_vsize		size;
  t_ia32_context	ctx;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);
  ASSERT(task_current(&task) == ERROR_NONE);

  ASSERT(ia32_get_context(caller, &ctx));

  type = ctx.eax;

  res = message_size(task, type, &size);

  ctx.eax = res;
  ctx.ebx = size;

  ASSERT(ia32_set_context(caller, &ctx, IA32_CONTEXT_EAX | IA32_CONTEXT_EBX));
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
  t_ia32_context	ctx;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);
  ASSERT(task_current(&task) == ERROR_NONE);

  ASSERT(ia32_get_context(caller, &ctx));

  u.reg[0] = ctx.eax;
  u.reg[1] = ctx.ebx;
  u.reg[2] = ctx.ecx;
  u.reg[3] = ctx.edx;
  type = ctx.esi;
  data = ctx.edi;
  size = ctx.ebp;

  res = message_send(task, u.node, type, data, size);

  ctx.eax = res;

  ASSERT(ia32_set_context(caller, &ctx, IA32_CONTEXT_EAX));
}

/*
 * handler for message_transmit syscall. message_transmit can block.
 */

void			ia32_syshandler_transmit(void)
{
  t_error		res;
  i_thread		caller;
  i_task		task;
  t_type		type;
  t_vaddr		data;
  t_vsize		size;
  t_ia32_context	ctx;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);
  ASSERT(task_current(&task) == ERROR_NONE);

  ASSERT(ia32_get_context(caller, &ctx));

  u.reg[0] = ctx.eax;
  u.reg[1] = ctx.ebx;
  u.reg[2] = ctx.ecx;
  u.reg[3] = ctx.edx;
  type = ctx.esi;
  data = ctx.edi;
  size = ctx.ebp;

  res = message_transmit(task, u.node, type, data, size);

  ctx.eax = res;

  ASSERT(ia32_set_context(caller, &ctx, IA32_CONTEXT_EAX));
}

/*
 * handler for message_receive syscall. message_receive can block.
 */

void			ia32_syshandler_receive(void)
{
  t_error		res;
  i_thread		caller;
  i_task		task;
  t_type		type;
  t_vaddr		data;
  t_vsize		size;
  t_ia32_context	ctx;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);
  ASSERT(task_current(&task) == ERROR_NONE);

  ASSERT(ia32_get_context(caller, &ctx));

  type = ctx.eax;
  data = ctx.ebx;

  res = message_receive(task, type, data, &size, &u.node);

  ctx.eax = res;
  ctx.ebx = size;
  ctx.ecx = u.reg[0];
  ctx.edx = u.reg[1];
  ctx.esi = u.reg[2];
  ctx.edi = u.reg[3];

  ASSERT(ia32_set_context(caller,
			  &ctx,
			  IA32_CONTEXT_EAX | IA32_CONTEXT_EBX |
			  IA32_CONTEXT_ECX | IA32_CONTEXT_EDX |
			  IA32_CONTEXT_ESI | IA32_CONTEXT_EDI));
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
  t_ia32_context	ctx;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  ASSERT(scheduler_current(&caller) == ERROR_NONE);
  ASSERT(task_current(&task) == ERROR_NONE);

  ASSERT(ia32_get_context(caller, &ctx));

  type = ctx.eax;
  data = ctx.ebx;

  res = message_poll(task, type, data, &size, &u.node);

  ctx.eax = res;
  ctx.ebx = size;
  ctx.ecx = u.reg[0];
  ctx.edx = u.reg[1];
  ctx.esi = u.reg[2];
  ctx.edi = u.reg[3];

  ASSERT(ia32_set_context(caller,
			  &ctx,
			  IA32_CONTEXT_EAX | IA32_CONTEXT_EBX |
			  IA32_CONTEXT_ECX | IA32_CONTEXT_EDX |
			  IA32_CONTEXT_ESI | IA32_CONTEXT_EDI));
}

/*
 * this function set the return code of a blocked syscall (message_transmit).
 * the thread must not be executing.
 */

t_error			ia32_syscall_set_code(i_thread		thread,
					      t_error		error)
{
  t_ia32_context	ctx;

  if (ia32_get_context(thread, &ctx) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ctx.eax = error;

  return (ia32_set_context(thread, &ctx, IA32_CONTEXT_EAX));
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
  t_ia32_context	ctx;
  union
  {
    i_node		node;
    t_uint32		reg[4];
  }			u;

  if (ia32_get_context(thread, &ctx) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  u.node = sender;

  ctx.eax = error;
  ctx.ebx = size;
  ctx.ecx = u.reg[0];
  ctx.edx = u.reg[1];
  ctx.esi = u.reg[2];
  ctx.edi = u.reg[3];

  return (ia32_set_context(thread,
			   &ctx,
			   IA32_CONTEXT_EAX | IA32_CONTEXT_EBX |
			   IA32_CONTEXT_ECX | IA32_CONTEXT_EDX |
			   IA32_CONTEXT_ESI | IA32_CONTEXT_EDI));
}

/*
 * this function registers the different syscalls.
 */

t_error			ia32_syscalls_init(void)
{
  if (event_reserve(56, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_register), 0) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (event_reserve(57, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_send), 0) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (event_reserve(58, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_transmit), 0) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /* XXX 59 */

  if (event_reserve(60, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_receive), 0) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /* XXX 61 */

  if (event_reserve(62, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_poll), 0) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /* XXX 63 */
  /* XXX 64 */

  if (event_reserve(65, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syshandler_size), 0) != ERROR_NONE)
    return (ERROR_UNKNOWN);

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
      event_release(64) != ERROR_NONE ||
      event_release(65) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/* [/cut] */
