/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/message/message.c
 *
 * created       julien quintard   [sat jul  1 23:25:14 2006]
 * updated       julian pidancet   [sat may  26 02:17:07 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the message manager provides a complete set of functions to send, receive,
 * request and reply messages in different passing modes.
 *
 * the message manager interface is equivalent in many ways to the
 * well-known parallel and distributed computing libraries like MPI, PVM etc.
 *
 * indeed, functions to send synchronously and asynchronously are provided
 * as well as blocking and non-blocking functions.
 *
 * sending an asynchronous message means the kaneton microkernel will have
 * to copy the message into an internal buffer and then to copy it out in
 * the destination buffer of the receiver.
 *
 * in the other hand, sending a synchronous message means that nothing is
 * done until the two parts, sender and receiver, are ready to exchange the
 * message. then, the kernel just copies the message from the source buffer
 * directly into the destination buffer.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

machdep_include(message);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * message manager variable.
 */

m_message*		message = NULL;

extern m_segment*	segment;
extern i_as		kasid;
extern i_task		ktask;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initialises the message manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 3) call the machine dependent code.
 */

t_error			message_init(void)
{
  /*
   * 1)
   */

  if ((message = malloc(sizeof(m_message))) == NULL)
    {
      cons_msg('!', "message: cannot allocate memory for the message "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(message, 0x0, sizeof(m_message));



  if (set_reserve(bpt, SET_OPT_SORT | SET_OPT_ALLOC, sizeof(t_message_box),
                  SEGMENT_BPT_NODESZ, &message->local_boxes) != ERROR_NONE)
  {
    cons_msg('!', "message: unable to reserve the msgboxes set.\n");
    return (ERROR_UNKNOWN);
  }

  if (message_register(ktask, 0) != ERROR_NONE)
  {
    cons_msg('!', "message: unable to register ktask msgbox.\n");
    return (ERROR_UNKNOWN);
  }

  /*
   * 2)
   */

  if (machdep_call(message, message_init) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * this function cleans the message manager.
 *
 * steps:
 *
 * 1) call the dependent code.
 * 2) release the statistics object.
 * 3) free the manager structure.
 */

t_error			message_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(message, message_clean) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_release(message->local_boxes) != ERROR_NONE)
  {
    free(message);
    return (ERROR_UNKNOWN);
  }

  free(message);

  return (ERROR_NONE);
}

/*
 * send asynchronously a message in a node msgbox
 *
 * steps:
 *
 * 1) allocate a buffer in the kernel as
 * 2) if the node is not on this machine (FIXME)
 * 3) copy the message from the given task's as to the kernel buffer
 * 4) enqueue the buffer into the node's messagebox
 */

t_error			message_async_send(i_task	sender,
					   i_node	dest,
					   t_tag	tag,
					   t_vaddr	data,
					   t_vsize	size)
{
  o_task*		sender_task;
  void*			kernel_buffer;
  o_message		msg;
  t_message_box*	msgbox;

  MESSAGE_ENTER(message);

  if (task_get(sender, &sender_task) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  /*
   * 1)
   */

  if (!(kernel_buffer = malloc(size)))
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (!is_local_node(dest))
  {
    /* XXX Redirect the message to the gate */
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  }

  /*
   * 3)
   */

  if (sender_task->asid != kasid)
  {
    if (as_read(sender_task->asid, data, size, kernel_buffer)
	  != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  }
  else
  {
    memcpy(kernel_buffer, (void *)data, size);
  }

  /*
   * 4)
   */

  msg.sz = size;
  msg.data = kernel_buffer;

  if (set_get(message->local_boxes, get_node_task(dest), (void**)&msgbox)
	!= ERROR_NONE)
  {
    free(kernel_buffer);
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  }

  tag = tag;

  if (set_push(msgbox->msgqueue, &msg) != ERROR_NONE)
  {
    free(kernel_buffer);
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  }

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * receive asynchronously a message from a node msgbox
 *
 * steps:
 *
 * 1) retrieve the message box and check if a message is pending, retrieve it.
 * 2) copy the message from the kernel buffer to the given task's as.
 * 3) pop the message from the queue and free kernel buffer.
 */

t_error			message_async_recv(i_task	taskid,
					   t_tag	tag,
					   t_vaddr	data,
					   t_vsize	maxsz)
{
  t_message_box*	msgbox;
  o_message*		msg;
  o_task*		task;

  MESSAGE_ENTER(message);

  /*
   * 1)
   */

  if (task_get(taskid, &task) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  if (set_get(message->local_boxes, taskid, (void**)&msgbox) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  tag = tag;

  if (set_pick(msgbox->msgqueue, (void**)&msg) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task->asid != kasid)
  {
    if (as_write(task->asid, msg->data, (maxsz < msg->sz ? maxsz : msg->sz),
	data) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  }
  else
  {
    memcpy((void*)data, (void*)msg->data,
	(maxsz < msg->sz ? maxsz : msg->sz));
  }

  /*
   * 3)
   */

  free(msg->data);

  if (set_pop(msgbox->msgqueue) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * send synchronously a message by direct memory transfert
 *
 * steps:
 *
 * 1) if the node is not on this machine, redirect the message through the gate (FIXME)
 * 2) retrieve the message box and check if a sync_recv is pending
 * 3) if not, register to the senders queue, block the thread and return
 *    (another thread will be elected while returning from isr).
 * 4) if it is, as-to-as mem copy, pop the receiver from the queue
 * 5) unblock the receiver's task
 */

t_error			message_sync_send(i_task	sender,
					  i_node	dest,
					  t_tag		tag,
					  t_vaddr	data,
					  t_vsize	size)
{
  t_message_box*	msgbox;
  o_task*		task;
  t_message_waiter*	receiver;
  t_message_waiter	send_waiter;
  i_thread		thread;
  t_size		sz;

  MESSAGE_ENTER(message);


  /*
   * 1)
   */

  if (!is_local_node(dest))
  {
    /* XXX Redirect the message to the gate */
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  }

  /*
   * 2)
   */

  if (task_get(sender, &task) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  if (set_get(message->local_boxes, get_node_task(dest), (void**)&msgbox)
      != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  tag = tag;

  if (set_pick(msgbox->receivers, (void**)&receiver) != ERROR_NONE)
  {
    /*
     * 3)
     */

    if (sched_current(&thread) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);

    send_waiter.thread = thread;
    send_waiter.data = data;
    send_waiter.sz = size;
    send_waiter.asid = task->asid;

    if (set_push(msgbox->senders, &send_waiter) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);

    if (thread_state(thread, SCHED_STATE_BLOCK) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);

    MESSAGE_LEAVE(message, ERROR_NONE);
  }

  /*
   * 4)
   */

  sz = size <= receiver->sz ? size : receiver->sz;

  if (receiver->asid == kasid && task->asid == kasid)
  {
    memcpy((void*)receiver->data, (void *)data, sz);
    goto copied;
  }

  if (receiver->asid == kasid)
  {
    if (as_read(task->asid, data, sz, (void*)receiver->data)
	!= ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
    goto copied;
  }

  if (task->asid == kasid)
  {
    if (as_write(receiver->asid, (void *)data, sz, receiver->data)
	!= ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
    goto copied;
  }

  if (as_copy(task->asid, data, receiver->asid, receiver->data, sz)
      != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

copied:

  if (set_pop(msgbox->receivers) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (machdep_call(message, message_epilogue, receiver->thread, ERROR_NONE)
        != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  if (thread_state(receiver->thread, SCHED_STATE_RUN) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * receive synchronously a message by direct memory transfert
 *
 * steps:
 *
 * 1) retrieve the message box and check if a sync_send is pending
 * 2) if no sync is pending, return if function not blocking.
 * 3) if no sync is pending and function blocking,
 *    register to the receivers queue, sleep & reschedule.
 * 4) if it is, pop the sender from the queue, proceed to transfert
 * 5) unblock the sender's task
 */

t_error			message_sync_recv(i_task	taskid,
					  t_tag		tag,
					  t_vaddr	data,
					  t_vsize	maxsz,
					  t_state	blocking)
{
  t_message_box*	msgbox;
  o_task*		task;
  t_message_waiter*	sender;
  t_message_waiter	recv_waiter;
  i_thread		thread;
  t_size		sz;

  MESSAGE_ENTER(message);

  /*
   * 1)
   */

  if (task_get(taskid, &task) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  if (set_get(message->local_boxes, taskid, (void**)&msgbox) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  tag = tag;

  if (set_pick(msgbox->senders, (void**)&sender) != ERROR_NONE)
  {
    /*
     * 2)
     */

    if (!blocking)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);

    /*
     * 3)
     */

    if (sched_current(&thread) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);

    recv_waiter.thread = thread;
    recv_waiter.data = data;
    recv_waiter.sz = maxsz;
    recv_waiter.asid = task->asid;

    if (set_push(msgbox->receivers, &recv_waiter) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);

    if (thread_state(thread, SCHED_STATE_BLOCK) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);

    MESSAGE_LEAVE(message, ERROR_NONE);
  }

  /*
   * 4)
   */

  sz = maxsz <= sender->sz ? maxsz : sender->sz;

  if (sender->asid == kasid && task->asid == kasid)
  {
    memcpy((void *)data, (void*)sender->data, sz);
    goto copied;
  }

  if (task->asid == kasid)
  {
    if (as_read(sender->asid, sender->data, sz, (void*)data) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
    goto copied;
  }

  if (sender->asid == kasid)
  {
    if (as_write(task->asid, (void*)sender->data, sz, data) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
    goto copied;
  }

  if (as_copy(sender->asid, sender->data, task->asid, data, sz)
      != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

copied:

  if (set_pop(msgbox->senders) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (machdep_call(message, message_epilogue, sender->thread, ERROR_NONE)
        != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  if (thread_state(sender->thread, SCHED_STATE_RUN) != ERROR_NONE)
    MESSAGE_LEAVE(message, ERROR_UNKNOWN);

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * register a message box for a task in the message manager
 *
 * steps:
 *
 * 1) create the message box if it doesn't exists
 */

t_error			message_register(i_task	taskid,
					 t_tag	tag)
{
  t_message_box*	msgbox;
  t_message_box		new;

  MESSAGE_ENTER(message);

  /*
   * 1)
   */

  if (set_get(message->local_boxes, taskid, (void**)&msgbox) != ERROR_NONE)
  {
    new.id = taskid;
    if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(o_message), &new.msgqueue)
	  != ERROR_NONE ||
	set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_message_waiter), &new.receivers)
	  != ERROR_NONE ||
	set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_message_waiter), &new.senders)
	  != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
    if (set_add(message->local_boxes, &new) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
    if (set_get(message->local_boxes, taskid, (void**)&msgbox) != ERROR_NONE)
      MESSAGE_LEAVE(message, ERROR_UNKNOWN);
  }

  MESSAGE_LEAVE(message, ERROR_NONE);
}

/*
 * ---------- tests -----------------------------------------------------------
 */


t_error			message_test(void)
{
  char			recv[128];

  MESSAGE_ENTER(message);

  sched_dump();

  while (1)
    {
      if (message_async_recv(ktask, 0, (t_vaddr)recv, 128) == ERROR_NONE)
	{
	  printf("received %d bytes : %s\n", strlen(recv) + 1, recv);
	  break;
	}
    }

  MESSAGE_LEAVE(message, ERROR_NONE);
}
