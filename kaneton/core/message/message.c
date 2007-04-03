/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/message/message.c
 *
 * created       julien quintard   [sat jul  1 23:25:14 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:37:56 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the message manager provides a complete set of functions to send, receive,
 * request and reply messages in different passing modes.
 *
 * the message manager interface is equivalent in many ways to the
 * well-known parallel and distributed computing libraries like MPI, PVM etc..
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

#include <klibc.h>
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

// FIXME For test
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
 * 2) initialise a statistic object.
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



  if (set_reserve(bpt, SET_OPT_SORT | SET_OPT_ALLOC, sizeof(t_local_box),
                  SEGMENT_BPT_NODESZ, &message->local_boxes) != ERROR_NONE)
  {
    cons_msg('!', "message: unable to reserve the msgboxes set\n");
    return (ERROR_UNKNOWN);
  }



  /*
   * 2)
   */

  STATS_RESERVE("message", &message->stats);

  /*
   * 3)
   */

  if (machdep_call(message, message_init) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);

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

  STATS_RELEASE(message->stats);

  /*
   * 3)
   */

  set_release(message->local_boxes);

  free(message);

  return (ERROR_NONE);
}


t_error			map_buffer(i_as asid, t_vaddr asvaddr, t_size size, void** mapping)
{
  o_segment*		seg;
  t_paddr		buff_paddr;
  t_iterator		i;
  t_size		offset;
  i_region		mappingid;
  o_region*		reg;

  as_paddr(asid, asvaddr, &buff_paddr);

  if (set_head(segment->segments, &i) != ERROR_NONE)
    return (ERROR_UNKNOWN);
  if (set_object(segment->segments, i, (void**)&seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  while (buff_paddr >= seg->address + seg->size)
  {
    if (set_next(segment->segments, i, &i) != ERROR_NONE)
      return (ERROR_UNKNOWN);
    if (set_object(segment->segments, i, (void**)&seg) != ERROR_NONE)
      return (ERROR_UNKNOWN);
  }
  if (buff_paddr < seg->address || buff_paddr >= seg->address + seg->size)
    return (ERROR_UNKNOWN);

  // FIXME: We actually force failure if the buffer is on more than one
  // segment
  if (buff_paddr + size > seg->address + seg->size)
    return (ERROR_UNKNOWN);

  offset = buff_paddr - seg->address;

  // FIXME: Optimize mapping size tweaking offset and size parameter
  if (region_reserve(kasid, seg->segid, 0, REGION_OPT_NONE, 0,
      seg->size, &mappingid) != ERROR_NONE)
    return (ERROR_UNKNOWN);
  if (region_get(kasid, mappingid, &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  *mapping = (void*)(reg->address + offset);

  return (ERROR_NONE);
}

t_error			unmap_buffer(void* ptr, t_size size)
{
  t_iterator		i;
  o_as*			kas;
  o_region*		reg;

  size = size;

  if (as_get(kasid, &kas) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (set_head(kas->regions, &i) != ERROR_NONE)
    return (ERROR_UNKNOWN);
  if (set_object(kas->regions, i, (void**)&reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  while ((t_vaddr)ptr >= reg->address + reg->size)
  {
    if (set_next(kas->regions, i, &i) != ERROR_NONE)
      return (ERROR_UNKNOWN);
    if (set_object(kas->regions, i, (void**)&reg) != ERROR_NONE)
      return (ERROR_UNKNOWN);
  }
  if ((t_vaddr)ptr < reg->address || (t_vaddr)ptr >= reg->address + reg->size)
    return (ERROR_UNKNOWN);

  if (region_release(kasid, reg->regid) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * Send asynchronously a message in a node msgbox
 *
 * 1) Get the data AS from the sender task id
 * 2) Temporary map the buffer into the kernel AS
 * 3) If the msgbox is not local, FIXME: pass the control to the gate
 * 4) in-kernel data copy through mapping
 * 5) Unmap the temporary zone
 * 6) Enqueue the message, in the node msgbox
 */

t_error			message_enqueue(i_task sender, i_node dest, t_tag tag, void* data, t_size size)
{
  o_task*		sender_task;
  void*			kernel_mapping;
  void*			kernel_buffer;
  o_msg			msg;
  t_local_box*		msgbox;

  /*
   * 1)
   */
  if (task_get(sender, &sender_task) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */
  if (sender_task->asid != kasid)
  {
    if (map_buffer(sender_task->asid, (t_vaddr)data, size, &kernel_mapping) != ERROR_NONE)
      return (ERROR_UNKNOWN);
  }
  else
    kernel_mapping = data;

  /*
   * 3)
   */
  if (!is_local_node(dest))
  {
    // FIXME: Send to the gate

    unmap_buffer(kernel_mapping, size);
    return (ERROR_NONE);
  }

  /*
   * 4)
   */
  if (!(kernel_buffer = malloc(size)))
    return (ERROR_UNKNOWN);
  memcpy(kernel_buffer, kernel_mapping, size);

  /*
   * 5)
   */
  if (sender_task->asid != kasid)
    if (unmap_buffer(kernel_mapping, size) != ERROR_NONE)
      return (ERROR_UNKNOWN);

  /*
   * 6)
   */
  msg.sz = size;
  msg.data = kernel_buffer;

  if (set_get(message->local_boxes, get_node_task(dest), (void**)&msgbox) != ERROR_NONE)
  {
    free(kernel_buffer);
    return (ERROR_UNKNOWN);
  }

  tag = tag;

  if (set_push(msgbox->queue, &msg) != ERROR_NONE)
  {
    free(kernel_buffer);
    return (ERROR_UNKNOWN);
  }

  return (ERROR_NONE);
}

/*
 * Receive asynchronously a message from a node msgbox
 *
 * 1) Retrieve the message box and check if a message is pending, retrieve it.
 * 2) Temporary map the buffer into kernel AS
 * 3) Data copy
 * 4) Unmap the buffer
 * 5) Pop the message from the queue and free kernel buffer
 */

t_error			message_pop(i_task taskid, t_tag tag, void* data, size_t maxsz)
{
  t_local_box*		msgbox;
  o_msg*		msg;
  o_task*		task;
  void*			kernel_mapping;

  /*
   * 1)
   */
  if (task_get(taskid, &task) != ERROR_NONE)
    return (ERROR_UNKNOWN);
  if (set_get(message->local_boxes, taskid, (void**)&msgbox) != ERROR_NONE)
    return (ERROR_UNKNOWN);
  tag = tag;
  if (set_pick(msgbox->queue, (void**)&msg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */
  if (task->asid != kasid)
  {
    if (map_buffer(task->asid, (t_vaddr)data, (maxsz < msg->sz ? maxsz : msg->sz), &kernel_mapping) != ERROR_NONE)
      return (ERROR_UNKNOWN);
  }
  else
    kernel_mapping = data;


  /*
   * 3)
   */
  memcpy(kernel_mapping, msg->data, (maxsz < msg->sz ? maxsz : msg->sz));

  /*
   * 4)
   */
  if (task->asid != kasid)
    if (unmap_buffer(kernel_mapping, (maxsz < msg->sz ? maxsz : msg->sz)) != ERROR_NONE)
      return (ERROR_UNKNOWN);

  /*
   * 5)
   */
  free(msg->data);
  set_pop(msgbox->queue);

  return (ERROR_NONE);
}

/*
 * Register a message box for a task in the message manager
 *
 * 1) Create the message box if it doesn't exists
 * 2) Add the given tag to the message box FIXME
 */
t_error			message_register(i_task taskid, t_tag tag)
{
  t_local_box*		msgbox;
  t_local_box		new;

  /*
   * 1)
   */

  if (set_get(message->local_boxes, taskid, (void**)&msgbox) != ERROR_NONE)
  {
    new.id = taskid;
    if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_local_box),
		    &new.queue) != ERROR_NONE)
      return (ERROR_UNKNOWN);
    if (set_add(message->local_boxes, &new) != ERROR_NONE)
      return (ERROR_UNKNOWN);
    if (set_get(message->local_boxes, taskid, (void**)&msgbox) != ERROR_NONE)
      return (ERROR_UNKNOWN);
  }

  /*
   * 2) FIXME
   */
  tag = tag;

  return (ERROR_NONE);
}

/*
 * ---------- tests -----------------------------------------------------------
 */


t_uint32		syscall_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size)
{
  t_uint32		ret;

  asm volatile("	pushl %%ebp		\n"
	       "	movl %7, %%ebp		\n"
	       "	int $56			\n"
	       "	popl %%ebp		"
	       : "=a" (ret)
	       : "a" (node[0]), "b" (node[1]), "c" (node[2]), "d" (node[3]),
	       "S" (tag), "D" (ptr), "m" (size));

  return (ret);
}

t_uint32		syscall_recv(t_uint32 tag, void* ptr, t_uint32 size)
{
  t_uint32		ret;

  asm volatile("	int $57			"
	       : "=a" (ret)
	       :"a" (tag), "b" (ptr), "c" (size));

  return (ret);
}

i_task tsk1;
i_task tsk2;

static void			chiche_send()
{
  char				tosend[128] =
    "Les chausettes de l'archiduchesse sont elles seches\n";
  i_node			task1_node;

  task1_node.machine = 2^64;
  task1_node.task = tsk2;

  syscall_send(&task1_node, 0, tosend, strlen(tosend) + 1);

  while (1)
    ;
}

static void			chiche_recv()
{
  char				recv[128];
  i_node			kernel_node;

  while (1)
  {
    if (syscall_recv(0, recv, 128) == ERROR_NONE)
    {
      break;
    }
  }

  kernel_node.machine = 2^64;
  kernel_node.task = 0;

  syscall_send(&kernel_node, 0, recv, strlen(recv) + 1);

  while (1)
    ;
}

t_error			message_test(void)
{
  char			recv[128];
  i_as			as1;
  i_as			as2;
  i_thread		th1;
  i_thread		th2;
  t_thread_context	ctx1;
  t_thread_context	ctx2;
  t_stack		stack1;
  t_stack		stack2;
  o_thread*		o1;
  o_thread*		o2;

  task_reserve(TASK_CLASS_PROGRAM,
	       TASK_BEHAV_INTERACTIVE,
	       TASK_PRIOR_INTERACTIVE,
	       &tsk1);
  task_reserve(TASK_CLASS_PROGRAM,
	       TASK_BEHAV_INTERACTIVE,
	       TASK_PRIOR_INTERACTIVE,
	       &tsk2);

  as_reserve(tsk1, &as1);
  as_reserve(tsk2, &as2);

  thread_reserve(tsk1, 150, &th1);
  thread_reserve(tsk2, 150, &th2);

  stack1.base = 0;
  stack1.size = THREAD_MIN_STACKSZ;
  stack2.base = 0;
  stack2.size = THREAD_MIN_STACKSZ;

  if (thread_stack(th1, stack1) != ERROR_NONE)
    {
      cons_msg('!', "cannot set stack\n");
      while (1);
    }
  if (thread_stack(th2, stack2) != ERROR_NONE)
    {
      cons_msg('!', "cannot set stack\n");
      while (1);
    }

  if (thread_get(th1, &o1) != ERROR_NONE)
    {
      cons_msg('!', "cannot get thread\n");
      while (1);
    }
  if (thread_get(th2, &o2) != ERROR_NONE)
    {
      cons_msg('!', "cannot get thread\n");
      while (1);
    }

  ctx1.sp = o1->stack + o1->stacksz - 16;
  ctx1.pc = (t_uint32)chiche_send;
  ctx2.sp = o2->stack + o2->stacksz - 16;
  ctx2.pc = (t_uint32)chiche_recv;

  if (thread_load(th1, ctx1) != ERROR_NONE)
    {
      cons_msg('!', "cannot load context\n");
      while (1);
    }
  if (thread_load(th2, ctx2) != ERROR_NONE)
    {
      cons_msg('!', "cannot load context\n");
      while (1);
    }

  if (message_register(ktask, 0) != ERROR_NONE)
  {
    cons_msg('!', "message test: message_register() error.\n");
    return ERROR_UNKNOWN;
  }
  if (message_register(tsk1, 0) != ERROR_NONE)
  {
    cons_msg('!', "message test: message_register() error.\n");
    return ERROR_UNKNOWN;
  }
  if (message_register(tsk2, 0) != ERROR_NONE)
  {
    cons_msg('!', "message test: message_register() error.\n");
    return ERROR_UNKNOWN;
  }

  task_state(tsk1, SCHED_STATE_RUN);
  task_state(tsk2, SCHED_STATE_RUN);

  sched_dump();

  while (1)
    {
      if (message_pop(ktask, 0, recv, 128) == ERROR_NONE)
	{
	  printf("task: %s\n", recv);
	}
    }

  return ERROR_NONE;
}
