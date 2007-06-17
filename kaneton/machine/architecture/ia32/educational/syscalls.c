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

static void		ia32_syscalls_async_send_handler(void)
{
  union
  {
    i_node		node;
    t_uint32		dword[4];
  }			u;
  t_vaddr		ptr;
  t_uint32		tag;
  t_uint32		size;
  i_task		source;
  t_uint32		ret;

  task_current(&source);

  u.dword[0] = context->eax;
  u.dword[1] = context->ebx;
  u.dword[2] = context->ecx;
  u.dword[3] = context->edx;
  tag = context->esi;
  ptr = context->edi;
  size = context->ebp;

  ret = message_async_send(source, u.node, tag, ptr, size);

  context->eax = ret;
}

static void		ia32_syscalls_sync_send_handler(void)
{
  union
  {
    i_node		node;
    t_uint32		dword[4];
  }			u;
  t_vaddr		ptr;
  t_uint32		tag;
  t_uint32		size;
  i_task		source;
  t_uint32		ret;
  i_thread		syscall, upcall;

  task_current(&source);

  sched_current(&syscall);

  u.dword[0] = context->eax;
  u.dword[1] = context->ebx;
  u.dword[2] = context->ecx;
  u.dword[3] = context->edx;
  tag = context->esi;
  ptr = context->edi;
  size = context->ebp;

  ret = message_sync_send(source, u.node, tag, ptr, size);

  sched_current(&upcall);

  if (syscall == upcall)
    context->eax = ret;
}

static void		ia32_syscalls_async_recv_handler(void)
{
  t_uint32		ret;
  t_uint32		tag;
  t_vaddr		ptr;
  t_uint32		size;
  i_task		source;

  task_current(&source);

  tag = context->eax;
  ptr = context->ebx;
  size = context->ecx;

  ret = message_async_recv(source, tag, ptr, size);

  context->eax = ret;
}

static void		ia32_syscalls_sync_recv_handler(void)
{
  t_uint32		ret;
  t_uint32		tag;
  t_vaddr		ptr;
  t_uint32		size;
  t_state		blocking;
  i_task		source;
  i_thread		syscall, upcall;

  task_current(&source);

  sched_current(&syscall);

  tag = context->eax;
  ptr = context->ebx;
  size = context->ecx;
  blocking = context->edx;

  ret = message_sync_recv(source, tag, ptr, size, blocking);

  sched_current(&upcall);

  if (syscall == upcall)
    context->eax = ret;
}

t_error			ia32_setup_syscall_ret(i_thread		thread,
					       t_error		exit_value)
{
  o_thread*		th;

  if (thread_get(thread, &th) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  th->machdep.context.eax = exit_value;

  return (ERROR_NONE);
}

t_error			ia32_syscalls_init(void)
{
  if (event_reserve(56, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syscalls_async_send_handler)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (event_reserve(57, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syscalls_async_recv_handler)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (event_reserve(58, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syscalls_sync_send_handler)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (event_reserve(59, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_syscalls_sync_recv_handler)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

t_error			ia32_syscalls_clean(void)
{
  if (event_release(56) != ERROR_NONE ||
      event_release(57) != ERROR_NONE ||
      event_release(58) != ERROR_NONE ||
      event_release(59) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k4 */
