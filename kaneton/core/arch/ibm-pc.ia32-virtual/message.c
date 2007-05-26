/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/message.c
 *
 * created       matthieu bucchianeri   [sat jun 17 19:38:22 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:41:19 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_message*	message;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the message manager dispatch.
 */

d_message		message_dispatch =
  {

/*                                                                  [cut] k4 */

    ia32_message_init,
    ia32_message_clean,
    ia32_message_epilogue

/*                                                                 [cut] /k4 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k4 */

void			ia32_message_async_send_handler(void)
{
  union
  {
    i_node		node;
    t_uint32		dword[4];
  }			u;
  void			*ptr;
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
  ptr = (void*)context->edi;
  size = context->ebp;

  ret = message_async_send(source, u.node, tag, ptr, size);

  context->eax = ret;
}

void			ia32_message_sync_send_handler(void)
{
  union
  {
    i_node		node;
    t_uint32		dword[4];
  }			u;
  void			*ptr;
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
  ptr = (void*)context->edi;
  size = context->ebp;

  ret = message_sync_send(source, u.node, tag, ptr, size);

  sched_current(&upcall);

  if (syscall == upcall)
    context->eax = ret;
}

void			ia32_message_async_recv_handler(void)
{
  t_uint32		ret;
  t_uint32		tag;
  void*			ptr;
  t_uint32		size;
  i_task		source;

  task_current(&source);

  tag = context->eax;
  ptr = (void*)context->ebx;
  size = context->ecx;

  ret = message_async_recv(source, tag, ptr, size);

  context->eax = ret;
}

void			ia32_message_sync_recv_handler(void)
{
  t_uint32		ret;
  t_uint32		tag;
  void*			ptr;
  t_uint32		size;
  t_uint32		flags;
  i_task		source;
  i_thread		syscall, upcall;

  task_current(&source);

  sched_current(&syscall);

  tag = context->eax;
  ptr = (void*)context->ebx;
  size = context->ecx;
  flags = context->edx;

  if (flags & 0x1)
    ret = message_sync_recv_nb(source, tag, ptr, size);
  else
    ret = message_sync_recv(source, tag, ptr, size);

  sched_current(&upcall);

  if (syscall == upcall)
    context->eax = ret;
}

t_error			ia32_message_epilogue(i_thread		thread,
					      t_error		exit_value)
{
  o_thread*		th;

  MESSAGE_ENTER(message);

  if (thread_get(thread, &th) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  th->machdep.context.eax = exit_value;

  MESSAGE_LEAVE(message, ERROR_NONE);
}

t_error			ia32_message_init(void)
{
  MESSAGE_ENTER(message);

  if (event_reserve(56, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_message_async_send_handler)) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  if (event_reserve(57, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_message_async_recv_handler)) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  if (event_reserve(58, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_message_sync_send_handler)) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  if (event_reserve(59, EVENT_FUNCTION,
		    EVENT_HANDLER(ia32_message_sync_recv_handler)) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

t_error			ia32_message_clean(void)
{
  MESSAGE_ENTER(message);

  if (event_release(56) != ERROR_NONE ||
      event_release(57) != ERROR_NONE ||
      event_release(58) != ERROR_NONE ||
      event_release(59) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*                                                                 [cut] /k4 */
