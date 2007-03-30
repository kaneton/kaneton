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

  if (asid == kasid)
  {
    *mapping = (void*) asvaddr;
    return (ERROR_NONE);
  }

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
  if (map_buffer(sender_task->asid, (t_vaddr)data, size, &kernel_mapping) != ERROR_NONE)
    return (ERROR_UNKNOWN);

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

t_error			message_pop(i_task taskid, t_tag tag, void* data)
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
  if (map_buffer(task->asid, (t_vaddr)data, msg->sz, &kernel_mapping) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */
  memcpy(kernel_mapping, msg->data, msg->sz);

  /*
   * 4)
   */
  if (unmap_buffer(kernel_mapping, msg->sz) != ERROR_NONE)
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


