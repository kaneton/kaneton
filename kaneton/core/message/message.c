/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/message/message.c
 *
 * created       matthieu bucchianeri   [mon jul 23 11:37:30 2007]
 * updated       julien quintard   [sun jan 30 20:10:24 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * [XXX:improvements] the whole manager should be re-developed!
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

#include <kaneton.h>

machine_include(message);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * kernel manager structure
 */

extern m_kernel		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * message manager variable.
 */

m_message		_message;

/*
 * ---------- functions -------------------------------------------------------
 */


/*
 * this function retrieves a message box of a task by its type.
 */

static t_status	message_box(i_task			task,
				    t_type			type,
				    o_message_type**		o)
{
  t_id			typeid = type;
  o_task*		otask;

  assert(o != NULL);

  if (task_get(task, &otask) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (set_get(otask->messages, typeid, (void**)o) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function registers a new message type with its maximum length.
 *
 * only registered types of messages can be sent through the kernel. unknown
 * types will be rejected.
 *
 * steps:
 *
 * 1) get the message box from the task.
 * 2) check if this type is already registered.
 * 3) build the message type structure.
 * 4) add it to the task messages set.
 * 5) call the machine dependent code.
 *
 */

t_status		message_register(i_task			task,
					 t_type			type,
					 t_vsize		size)
{
  t_id			typeid = type;
  o_task*		o;
  o_message_type	msgtype;

  /*
   * 1)
   */

  if (task_get(task, &o) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (set_exist(o->messages, typeid) == TRUE)
    CORE_ESCAPE("XXX");

  /*
   * 3)
   */

  msgtype.id = 0;
  msgtype.type = typeid;
  msgtype.size = size;

  if (set_reserve(pipe, SET_OPTION_ALLOCATE, sizeof (o_message),
		  &msgtype.queue) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (set_reserve(pipe, SET_OPTION_ALLOCATE, sizeof (o_message),
		  &msgtype.waiters) != STATUS_OK)
    {
      set_release(msgtype.queue);

      CORE_ESCAPE("XXX");
    }

  /*
   * 4)
   */

  if (set_add(o->messages, &msgtype) != STATUS_OK)
    {
      set_release(msgtype.waiters),
      set_release(msgtype.queue);

      CORE_ESCAPE("XXX");
    }

  /*
   * 5)
   */

  if (machine_call(message, register, task, type, size) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function returns the maximum size of a message in a message box.
 *
 */

t_status		message_size(i_task			task,
				     t_type			type,
				     t_vsize*			size)
{
  o_message_type*	o;

  if (message_box(task, type, &o) != STATUS_OK)
    CORE_ESCAPE("XXX");

  *size = o->size;

  CORE_LEAVE();
}

/*
 * this function free all the message types for one task.
 *
 * steps:
 *
 * 1) loop though the message types.
 * 2) unblock waiters.
 * 3) free outgoing messages.
 * 4) release the sets.
 * 5) call machine.
 *
 */

t_status		message_flush(i_task			task)
{
  o_task*		o;
  o_message_type*	otype;
  o_message*		omsg;
  s_iterator		it;

  if (task_get(task, &o) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 1)
   */

  while (set_head(o->messages, &it) == TRUE)
    {
      if (set_object(o->messages, it, (void**)&otype) != STATUS_OK)
	CORE_ESCAPE("XXX");

      /*
       * 2)
       */

      while (set_pick(otype->waiters, (void**)&omsg) != STATUS_OK)
	{
	  if (message_return(omsg->blocked, STATUS_ERROR) != STATUS_OK)
	    CORE_ESCAPE("XXX");

	  if (set_pop(otype->waiters) != STATUS_OK)
	    CORE_ESCAPE("XXX");
	}

      /*
       * 3)
       */

      while (set_pick(otype->queue, (void**)&omsg) != STATUS_OK)
	{
	  if (set_pop(otype->waiters) != STATUS_OK)
	    CORE_ESCAPE("XXX");

	  if (omsg->as == ID_UNUSED)
	    {
	      if (message_return(omsg->blocked, STATUS_ERROR) != STATUS_OK)
		CORE_ESCAPE("XXX");
	    }
	  else
	    {
	      if (omsg->size != 0)
		free(omsg->data);
	    }
	}

      /*
       * 4)
       */

      if (set_release(otype->waiters) != STATUS_OK)
	CORE_ESCAPE("XXX");

      if (set_release(otype->queue) != STATUS_OK)
	CORE_ESCAPE("XXX");
    }

  /*
   * 5)
   */

  if (machine_call(message, flush, task) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function sends a message to a task. the message is delivered
 * asynchronously and the function returns only when the buffer was safely
 * copied.
 *
 * steps:
 *
 * 1) check message destination.
 * 2) get the destination message box.
 * 3) check the size.
 * 4) system call special case.
 * 5) if a thread is waiting for a message, then send it synchronously.
 * 6) build the message.
 *  a) fill the buffer, intra-kernel case.
 *  b) fill the buffer, inter-as case.
 * 7) push the message into the message box.
 * 8) call the machine dependent code.
 *
 */

t_status		message_send(i_task			task,
				     i_node			destination,
				     t_type			type,
				     t_vaddr			data,
				     t_vsize			size)
{
  t_id			typeid = type;
  o_message_type*	o;
  o_task*		otsk;
  o_message		msg;
  t_setsz		setsz;

  /*
   * 1)
   */

  if (destination.cell != _kernel.cell)
    {
      /* XXX distr */
      CORE_ESCAPE("XXX");
    }

  /*
   * 2)
   */

  if (message_box(destination.task, typeid, &o) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 3)
   */

  if (size > o->size)
    CORE_ESCAPE("XXX");

  /*
   * 4)
   */

  if (destination.task == _kernel.task && type == 0)
    {
      void*		buffer;
      i_node		source;

      assert(task != _kernel.task);
      assert(size != 0);

      source.cell = _kernel.cell;
      source.task = task;

      if (task_get(task, &otsk) != STATUS_OK)
	CORE_ESCAPE("XXX");

      if ((buffer = malloc(size)) == NULL)
	CORE_ESCAPE("XXX");

      if (as_read(otsk->as, data, size, buffer) != STATUS_OK)
	{
	  free(buffer);

	  CORE_ESCAPE("XXX");
	}

      if (interface_notify(buffer, size, source) != STATUS_OK)
	CORE_ESCAPE("XXX");

      free(buffer);

      CORE_LEAVE();
    }

  /*
   * 5)
   */

  if (set_size(o->waiters, &setsz) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (setsz != 0)
    {
      if (message_transmit(task, destination, type, data, size) != STATUS_OK)
	CORE_ESCAPE("XXX");

      CORE_LEAVE();
    }

  /*
   * 6)
   */

  if (size)
    {
      if ((msg.data = malloc(size)) == NULL)
	CORE_ESCAPE("XXX");
    }
  else
    msg.data = NULL;

  if (size)
    {
      if (task == _kernel.task)
	{
	  /*
	   * a)
	   */

	  memcpy(msg.data, (void*)data, size);
	}
      else
	{
	  /*
	   * b)
	   */

	  if (task_get(task, &otsk) != STATUS_OK)
	    {
	      free(msg.data);

	      CORE_ESCAPE("XXX");
	    }

	  if (as_read(otsk->as, data, size, msg.data) != STATUS_OK)
	    {
	      free(msg.data);

	      CORE_ESCAPE("XXX");
	    }
	}
    }

  msg.as = ID_UNUSED;
  msg.size = size;
  msg.sender.cell = _kernel.cell;
  msg.sender.task = task;

  /*
   * 7)
   */

  msg.id = o->id++;

  if (set_push(o->queue, &msg) != STATUS_OK)
    {
      if (msg.data != NULL)
	free(msg.data);

      CORE_ESCAPE("XXX");
    }

  /*
   * 8)
   */

  if (machine_call(message, send, task, destination, type,
		   data, size) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function sends a message to a task. the message is delivered
 * synchronously and the function returns only when the message is delivered
 * correctly.
 *
 * there is no kernel bufferization.
 *
 * steps:
 *
 * 1) check for message destination.
 * 2) get message box and task object.
 * 3) check size.
 * 4) do the transmission.
 *  a) immediately if a message is being waited.
 *  b) later if no rendezvous has been established.
 * 5) call machine dependent code.
 *
 */

t_status		message_transmit(i_task			task,
					 i_node			destination,
					 t_type			type,
					 t_vaddr		data,
					 t_vsize		size)
{
  t_id			typeid = type;
  o_message_type*	o;
  o_task*		otsk;
  o_message		msg;
  o_message*		pmsg;
  t_setsz		setsz;
  i_thread		thread;

  /*
   * 1)
   */

  if (destination.cell != _kernel.cell)
    {
      /* XXX distr */
      CORE_ESCAPE("XXX");
    }

  /*
   * 2)
   */

  if (message_box(destination.task, typeid, &o) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (task_get(task, &otsk) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 3)
   */

  if (size > o->size)
    CORE_ESCAPE("XXX");

  /*
   * 4)
   */

  if (set_size(o->waiters, &setsz) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (setsz != 0)
    {
      i_node		sender;

      /*
       * a)
       */

      if (set_pick(o->waiters, (void*)&pmsg) != STATUS_OK)
	CORE_ESCAPE("XXX");

      if (size)
	if (as_copy(otsk->as, data, pmsg->as, (t_vaddr)pmsg->data,
		    size) != STATUS_OK)
	  CORE_ESCAPE("XXX");

      sender.cell = _kernel.cell;
      sender.task = task;

      if (message_return_info(pmsg->blocked, STATUS_OK, size,
			      sender) != STATUS_OK)
	CORE_ESCAPE("XXX");

      if (set_pop(o->waiters) != STATUS_OK)
	CORE_ESCAPE("XXX");
    }
  else
    {
      /*
       * b)
       */

      if (thread_current(&thread) != STATUS_OK)
	CORE_ESCAPE("XXX");

      msg.as = otsk->as;
      msg.data = (void*)data;
      msg.size = size;
      msg.sender.cell = _kernel.cell;
      msg.sender.task = task;
      msg.blocked = thread;

      msg.id = o->id++;

      if (set_push(o->queue, &msg) != STATUS_OK)
	CORE_ESCAPE("XXX");

      if (thread_block(thread) != STATUS_OK)
	CORE_ESCAPE("XXX");
    }

  /*
   * 5)
   */

  if (machine_call(message, transmit, task, destination, type,
		   data, size) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function sends a message to a task. the message is delivered
 * asynchronously and the function returns immediately. the request parameter
 * is used to poll or wait until the buffer is safely copied.
 *
 * on non-multiprocessor systems, this function is totally equivalent to
 * message_send.
 *
 */

t_status		message_throw(i_task			task,
				      i_node			destination,
				      t_type			type,
				      t_vaddr			data,
				      t_vsize			size,
				      s_message_request*	request)
{
  assert(request != NULL);

  if (cpu_multiprocessor() == TRUE)
    {
      /* XXX smp */

      CORE_ESCAPE("XXX");
    }
  else
    {
      t_status	res;

      res = message_send(task, destination, type, data, size);

      if (res != STATUS_OK)
	CORE_ESCAPE("XXX");

      request->completed = MESSAGE_REQUEST_COMPLETED;
    }

  CORE_LEAVE();
}

/*
 * this function waits and receives a message, whether it is delivered
 * asynchronously or synchronously.
 *
 * steps:
 *
 * 1) get the message box.
 * 2) look for pending message.
 *  a) no pending message, block.
 *  b) incoming message, get it.
 * 3) call machine code.
 *
 */

t_status		message_receive(i_task			task,
					t_type			type,
					t_vaddr			data,
					t_vsize*		size,
					i_node*			sender)
{
  t_id			typeid = type;
  o_message_type*	o;
  t_setsz		setsz;
  i_thread		thread;
  o_message		msg;
  o_task*		otsk;

  /*
   * 1)
   */

  if (message_box(task, typeid, &o) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (set_size(o->queue, &setsz) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (setsz == 0)
    {
      /*
       * a)
       */

      if (task_get(task, &otsk) != STATUS_OK)
	CORE_ESCAPE("XXX");

      if (thread_current(&thread) != STATUS_OK)
	CORE_ESCAPE("XXX");

      msg.id = o->id++;
      msg.as = otsk->as;
      msg.data = (void*)data;
      msg.blocked = thread;

      if (set_push(o->waiters, &msg) != STATUS_OK)
	CORE_ESCAPE("XXX");

      if (thread_block(thread) != STATUS_OK)
	CORE_ESCAPE("XXX");
    }
  else
    {
      /*
       * b)
       */

      if (message_poll(task, type, data, size, sender) != STATUS_OK)
	CORE_ESCAPE("XXX");

      CORE_LEAVE();
    }

  /*
   * 3)
   */

  if (machine_call(message, poll, task, type, data,
		   size, sender) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function starts receiving asynchronous messages. the function returns
 * immediately, so it is not safe to use the buffer until the request has been
 * completely executed.
 *
 * on non-multiprocessor systems, this function is stricly equivalent to
 * message_receive.
 *
 */

t_status		message_grab(i_task			task,
				     t_type			type,
				     t_vaddr			data,
				     s_message_request*		request)
{
  if (cpu_multiprocessor() == TRUE)
    {
      /* XXX smp */

      CORE_ESCAPE("XXX");
    }
  else
    {
      t_status	res;
      t_vsize		size;
      i_node		sender;

      res = message_receive(task, type, data, &size, &sender);

      if (res != STATUS_OK)
	CORE_ESCAPE("XXX");

      request->completed = MESSAGE_REQUEST_COMPLETED;
      /*      request->size = size;
	      request->sender = sender;*/
    }

  CORE_LEAVE();
}

/*
 * this function checks for an incoming message, receives it or returns an
 * error immediately. this function can be used with asynchronous ou
 * synchronous messages.
 *
 * steps:
 *
 * 1) get the destination message box.
 * 2) poll for incoming message.
 * 3) treat the message.
 *  a) synchronous case.
 *  b) asynchronous, intra-kernel case.
 *  c) asynchronous, inter-address space case.
 * 4) call the machine dependent code.
 *
 */

t_status		message_poll(i_task			task,
				     t_type			type,
				     t_vaddr			data,
				     t_vsize*			size,
				     i_node*			sender)
{
  t_id			typeid = type;
  o_message_type*	o;
  o_task*		otsk;
  o_message*		msg;

  /*
   * 1)
   */

  if (message_box(task, typeid, &o) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 2
   */

  if (set_pick(o->queue, (void*)&msg) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 3)
   */

  if (msg->as != ID_UNUSED)
    {
      /*
       * a)
       */

      if (msg->size)
	{
	  if (task_get(task, &otsk) != STATUS_OK)
	    CORE_ESCAPE("XXX");

	  if (as_copy(msg->as, (t_vaddr)msg->data, otsk->as, data,
		      msg->size) != STATUS_OK)
	    CORE_ESCAPE("XXX");
	}

      if (message_return(msg->blocked, STATUS_OK) != STATUS_OK)
	CORE_ESCAPE("XXX");
    }
  else
    {
      if (task == _kernel.task)
	{
	  /*
	   * b)
	   */

	  if (msg->size)
	    memcpy((void*)data, msg->data, msg->size);
	}
      else
	{
	  /*
	   * c)
	   */

	  if (msg->size)
	    {
	      if (task_get(task, &otsk) != STATUS_OK)
		CORE_ESCAPE("XXX");

	      if (as_write(otsk->as, msg->data, msg->size,
			   data) != STATUS_OK)
		CORE_ESCAPE("XXX");
	    }
	}

      if (msg->data != NULL)
	free(msg->data);
    }

  *size = msg->size;
  *sender = msg->sender;

  if (set_pop(o->queue) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 4)
   */

  if (machine_call(message, poll, task, type, data,
		   size, sender) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function checks if a message copy (though message_throw or
 * message_grab) is finished yet.
 *
 */

t_status		message_state(i_task			task,
				      s_message_request		request)
{
  /* XXX smp */

  CORE_ESCAPE("XXX");
}

/*
 * this function checks if a message copy (though message_throw or
 * message_grab) is finished yet and waits for the operation to be completed.
 *
 */

t_status		message_wait(i_task			task,
				     s_message_request		request,
				     t_vsize*			size,
				     i_node*			sender)
{
  /* XXX smp */

  CORE_ESCAPE("XXX");
}

/*
 * this function resumes a blocked thread on a syscall with given return value.
 *
 */

t_status		message_return(i_thread			thread,
				       t_status		code)
{
  if (machine_call(message, return, thread, code) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (thread_start(thread) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function resumes a blocked thread on a syscall with given info.
 *
 */

t_status		message_return_info(i_thread		thread,
					    t_status	code,
					    t_vsize		size,
					    i_node		sender)
{
  if (machine_call(message, return_info, thread, code, size,
		   sender) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (thread_start(thread) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}


/*
 * this function initializes the message manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) create message types for the kernel task.
 * 3) call the machine dependent code.
 *
 */

t_status		message_initialize(void)
{
  /*
   * XXX
   */

  module_call(console, message,
	      '+', "initializing the message manager\n");

  /*
   * 1)
   */

  memset(&_message, 0x0, sizeof (m_message));

  /*
   * 2)
   */

  if (message_register(_kernel.task, MESSAGE_TYPE_INTERFACE,
		       sizeof (o_syscall)) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (message_register(_kernel.task, MESSAGE_TYPE_EVENT,
		       sizeof (o_event_message)) != STATUS_OK)
    CORE_ESCAPE("XXX");

  if (message_register(_kernel.task, MESSAGE_TYPE_TIMER,
		       sizeof (o_timer_message)) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 3)
   */

  if (machine_call(message, initialize) != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}

/*
 * this function cleans the message manager.
 *
 * steps:
 *
 * 1) call the dependent code.
 * 3) free kernel message types.
 * 2) free the manager structure.
 *
 */

t_status		message_clean(void)
{
  /*
   * XXX
   */

  module_call(console, message,
	      '+', "cleaning the message manager\n");

  /*
   * 1)
   */

  if (machine_call(message, clean) != STATUS_OK)
    CORE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (message_flush(_kernel.task)  != STATUS_OK)
    CORE_ESCAPE("XXX");

  CORE_LEAVE();
}
