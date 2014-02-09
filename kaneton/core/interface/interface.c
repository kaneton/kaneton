/*
 * ---------- information -----------------------------------------------------
 *
 * [XXX:improvements] the whole manager should be re-developed!
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- functions -------------------------------------------------------
 */
/*
 * this function launchs the as_copy() function.
 */

t_status	interface_as_copy(o_syscall*	message)
{
  t_status error;

  error = as_copy(message->u.request.u.as_copy.arg1,
			message->u.request.u.as_copy.arg2,
			message->u.request.u.as_copy.arg3,
			message->u.request.u.as_copy.arg4,
			message->u.request.u.as_copy.arg5);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the as_release() function.
 */

t_status	interface_as_release(o_syscall*	message)
{
  t_status error;

  error = as_release(message->u.request.u.as_release.arg1);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function get the task attribute of the o_as object.
 */

t_status	interface_as_attribute_task(o_syscall*	message)
{
  o_as*		o;

  if (as_get(message->u.request.u.as_attribute_task.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.as_attribute_task.result1 = o->task;
    }

  return (STATUS_OK);
}

/*
 * this function launchs the event_reserve() function.
 */

t_status	interface_event_reserve(o_syscall*	message)
{
  t_status error;

  error = event_reserve(message->u.request.u.event_reserve.arg1,
			message->u.request.u.event_reserve.arg2,
			message->u.request.u.event_reserve.arg3,
			message->u.request.u.event_reserve.arg4);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the event_release() function.
 */

t_status	interface_event_release(o_syscall*	message)
{
  t_status error;

  error = event_release(message->u.request.u.event_release.arg1);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function get the type attribute of the o_event object.
 */

t_status	interface_event_attribute_type(o_syscall*	message)
{
  o_event*		o;

  if (event_get(message->u.request.u.event_attribute_type.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.event_attribute_type.result1 = o->type;
    }

  return (STATUS_OK);
}

/*
 * this function get the handler attribute of the o_event object.
 */

t_status	interface_event_attribute_handler(o_syscall*	message)
{
  o_event*		o;

  if (event_get(message->u.request.u.event_attribute_handler.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.event_attribute_handler.result1 = o->handler;
    }

  return (STATUS_OK);
}

/*
 * this function get the data attribute of the o_event object.
 */

t_status	interface_event_attribute_data(o_syscall*	message)
{
  o_event*		o;

  if (event_get(message->u.request.u.event_attribute_data.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.event_attribute_data.result1 = o->data;
    }

  return (STATUS_OK);
}

/*
 * this function launchs the map_reserve() function.
 */

t_status	interface_map_reserve(o_syscall*	message)
{
  t_status error;
  t_vaddr	result1;

  error = map_reserve(message->u.request.u.map_reserve.arg1,
			message->u.request.u.map_reserve.arg2,
			message->u.request.u.map_reserve.arg3,
			message->u.request.u.map_reserve.arg4,
			&result1);

  message->u.reply.error = error;
  message->u.reply.u.map_reserve.result1 = result1;

  return (STATUS_OK);
}

/*
 * this function launchs the map_release() function.
 */

t_status	interface_map_release(o_syscall*	message)
{
  t_status error;

  error = map_release(message->u.request.u.map_release.arg1,
			message->u.request.u.map_release.arg2);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the map_resize() function.
 */

t_status	interface_map_resize(o_syscall*	message)
{
  t_status error;
  t_vaddr	result1;

  error = map_resize(message->u.request.u.map_resize.arg1,
			message->u.request.u.map_resize.arg2,
			message->u.request.u.map_resize.arg3,
			&result1);

  message->u.reply.error = error;
  message->u.reply.u.map_resize.result1 = result1;

  return (STATUS_OK);
}

/*
 * this function launchs the region_reserve() function.
 */

t_status	interface_region_reserve(o_syscall*	message)
{
  t_status error;
  i_region	result1;

  error = region_reserve(message->u.request.u.region_reserve.arg1,
			message->u.request.u.region_reserve.arg2,
			message->u.request.u.region_reserve.arg3,
			message->u.request.u.region_reserve.arg4,
			message->u.request.u.region_reserve.arg5,
			message->u.request.u.region_reserve.arg6,
			&result1);

  message->u.reply.error = error;
  message->u.reply.u.region_reserve.result1 = result1;

  return (STATUS_OK);
}

/*
 * this function launchs the region_release() function.
 */

t_status	interface_region_release(o_syscall*	message)
{
  t_status error;

  error = region_release(message->u.request.u.region_release.arg1,
			message->u.request.u.region_release.arg2);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the scheduler_quantum() function.
 */

t_status	interface_scheduler_quantum(o_syscall*	message)
{
  t_status error;

  error = scheduler_quantum(message->u.request.u.scheduler_quantum.arg1);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the scheduler_yield() function.
 */

t_status	interface_scheduler_yield(o_syscall*	message)
{
  t_status error;

  error = scheduler_yield();

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the scheduler_update() function.
 */

t_status	interface_scheduler_update(o_syscall*	message)
{
  t_status error;

  error = scheduler_update(message->u.request.u.scheduler_update.arg1);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the segment_clone() function.
 */

t_status	interface_segment_clone(o_syscall*	message)
{
  t_status error;
  i_segment	result1;

  error = segment_clone(message->u.request.u.segment_clone.arg1,
			message->u.request.u.segment_clone.arg2,
			&result1);

  message->u.reply.error = error;
  message->u.reply.u.segment_clone.result1 = result1;

  return (STATUS_OK);
}

/*
 * this function launchs the segment_give() function.
 */

t_status	interface_segment_give(o_syscall*	message)
{
  t_status error;

  error = segment_give(message->u.request.u.segment_give.arg1,
			message->u.request.u.segment_give.arg2);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the segment_copy() function.
 */

t_status	interface_segment_copy(o_syscall*	message)
{
  t_status error;

  error = segment_copy(message->u.request.u.segment_copy.arg1,
			message->u.request.u.segment_copy.arg2,
			message->u.request.u.segment_copy.arg3,
			message->u.request.u.segment_copy.arg4,
			message->u.request.u.segment_copy.arg5);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the segment_release() function.
 */

t_status	interface_segment_release(o_syscall*	message)
{
  t_status error;

  error = segment_release(message->u.request.u.segment_release.arg1);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the segment_perms() function.
 */

t_status	interface_segment_permissions(o_syscall*	message)
{
  t_status error;

  error = segment_permissions(message->u.request.u.segment_permissions.arg1,
			      message->u.request.u.segment_permissions.arg2);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function get the asid attribute of the o_segment object.
 */

t_status	interface_segment_attribute_as(o_syscall*	message)
{
  o_segment*		o;

  if (segment_get(message->u.request.u.segment_attribute_as.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.segment_attribute_as.result1 = o->as;
    }

  return (STATUS_OK);
}

/*
 * this function get the address attribute of the o_segment object.
 */

t_status	interface_segment_attribute_address(o_syscall*	message)
{
  o_segment*		o;

  if (segment_get(message->u.request.u.segment_attribute_address.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.segment_attribute_address.result1 = o->address;
    }

  return (STATUS_OK);
}

/*
 * this function get the size attribute of the o_segment object.
 */

t_status	interface_segment_attribute_size(o_syscall*	message)
{
  o_segment*		o;

  if (segment_get(message->u.request.u.segment_attribute_size.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.segment_attribute_size.result1 = o->size;
    }

  return (STATUS_OK);
}

/*
 * this function get the perms attribute of the o_segment object.
 */

t_status	interface_segment_attribute_permissions(o_syscall*	message)
{
  o_segment*		o;

  if (segment_get(message->u.request.u.segment_attribute_permissions.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.segment_attribute_permissions.result1 = o->permissions;
    }

  return (STATUS_OK);
}

/*
 * this function launchs the task_reserve() function.
 */

t_status	interface_task_reserve(o_syscall*	message)
{
  t_status error;
  i_task	result1;

  error = task_reserve(message->u.request.u.task_reserve.arg1,
			message->u.request.u.task_reserve.arg2,
			message->u.request.u.task_reserve.arg3,
			&result1);

  message->u.reply.error = error;
  message->u.reply.u.task_reserve.result1 = result1;

  return (STATUS_OK);
}

/*
 * this function launchs the task_release() function.
 */

t_status	interface_task_release(o_syscall*	message)
{
  t_status error;

  error = task_release(message->u.request.u.task_release.arg1);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the task_priority() function.
 */

t_status	interface_task_priority(o_syscall*	message)
{
  t_status error;

  error = task_priority(message->u.request.u.task_priority.arg1,
			message->u.request.u.task_priority.arg2);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function get the parent attribute of the o_task object.
 */

t_status	interface_task_attribute_parent(o_syscall*	message)
{
  o_task*		o;

  if (task_get(message->u.request.u.task_attribute_parent.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.task_attribute_parent.result1 = o->parent;
    }

  return (STATUS_OK);
}

/*
 * this function get the class attribute of the o_task object.
 */

t_status	interface_task_attribute_class(o_syscall*	message)
{
  o_task*		o;

  if (task_get(message->u.request.u.task_attribute_class.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.task_attribute_class.result1 = o->class;
    }

  return (STATUS_OK);
}

/*
 * this function get the behav attribute of the o_task object.
 */

t_status	interface_task_attribute_behaviour(o_syscall*	message)
{
  o_task*		o;

  if (task_get(message->u.request.u.task_attribute_behaviour.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.task_attribute_behaviour.result1 = o->behaviour;
    }

  return (STATUS_OK);
}

/*
 * this function get the prior attribute of the o_task object.
 */

t_status	interface_task_attribute_priority(o_syscall*	message)
{
  o_task*		o;

  if (task_get(message->u.request.u.task_attribute_priority.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.task_attribute_priority.result1 = o->priority;
    }

  return (STATUS_OK);
}

/*
 * this function get the as attribute of the o_task object.
 */

t_status	interface_task_attribute_as(o_syscall*	message)
{
  o_task*		o;

  if (task_get(message->u.request.u.task_attribute_as.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.task_attribute_as.result1 = o->as;
    }

  return (STATUS_OK);
}

/*
 * this function get the sched attribute of the o_task object.
 */

t_status	interface_task_attribute_sched(o_syscall*	message)
{
  o_task*		o;

  if (task_get(message->u.request.u.task_attribute_sched.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.task_attribute_sched.result1 = o->state;
    }

  return (STATUS_OK);
}

/*
 * this function launchs the thread_release() function.
 */

t_status	interface_thread_release(o_syscall*	message)
{
  t_status error;

  error = thread_release(message->u.request.u.thread_release.arg1);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the thread_priority() function.
 */

t_status	interface_thread_priority(o_syscall*	message)
{
  t_status error;

  error = thread_priority(message->u.request.u.thread_priority.arg1,
			message->u.request.u.thread_priority.arg2);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the thread_load() function.
 */

t_status	interface_thread_load(o_syscall*	message)
{
  t_status error;

  error = thread_load(message->u.request.u.thread_load.arg1,
			message->u.request.u.thread_load.arg2);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function launchs the thread_store() function.
 */

t_status	interface_thread_store(o_syscall*	message)
{
  t_status error;
  s_thread_context	result1;

  error = thread_store(message->u.request.u.thread_store.arg1,
			&result1);

  message->u.reply.error = error;
  message->u.reply.u.thread_store.result1 = result1;

  return (STATUS_OK);
}

/*
 * this function get the taskid attribute of the o_thread object.
 */

t_status	interface_thread_attribute_task(o_syscall*	message)
{
  o_thread*		o;

  if (thread_get(message->u.request.u.thread_attribute_task.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.thread_attribute_task.result1 = o->task;
    }

  return (STATUS_OK);
}

/*
 * this function get the prior attribute of the o_thread object.
 */

t_status	interface_thread_attribute_priority(o_syscall*	message)
{
  o_thread*		o;

  if (thread_get(message->u.request.u.thread_attribute_priority.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.thread_attribute_priority.result1 = o->priority;
    }

  return (STATUS_OK);
}

/*
 * this function get the sched attribute of the o_thread object.
 */

t_status	interface_thread_attribute_state(o_syscall*	message)
{
  o_thread*		o;

  if (thread_get(message->u.request.u.thread_attribute_state.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.thread_attribute_state.result1 = o->state;
    }

  return (STATUS_OK);
}

/*
 * this function launchs the timer_reserve() function.
 */

t_status	interface_timer_reserve(o_syscall*	message)
{
  t_status error;
  i_timer	result1;

  error = timer_reserve(message->u.request.u.timer_reserve.arg1,
			message->u.request.u.timer_reserve.arg2,
			message->u.request.u.timer_reserve.arg3,
			message->u.request.u.timer_reserve.arg4,
			message->u.request.u.timer_reserve.arg5,
			&result1);

  message->u.reply.error = error;
  message->u.reply.u.timer_reserve.result1 = result1;

  return (STATUS_OK);
}

/*
 * this function launchs the timer_release() function.
 */

t_status	interface_timer_release(o_syscall*	message)
{
  t_status error;

  error = timer_release(message->u.request.u.timer_release.arg1);

  message->u.reply.error = error;

  return (STATUS_OK);
}

/*
 * this function get the delay attribute of the o_timer object.
 */

t_status	interface_timer_attribute_delay(o_syscall*	message)
{
  o_timer*		o;

  if (timer_get(message->u.request.u.timer_attribute_delay.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.timer_attribute_delay.result1 = o->delay;
    }

  return (STATUS_OK);
}

/*
 * this function get the repeat attribute of the o_timer object.
 */

t_status	interface_timer_attribute_repeat(o_syscall*	message)
{
  o_timer*		o;

  if (timer_get(message->u.request.u.timer_attribute_repeat.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.timer_attribute_repeat.result1 = o->repeat;
    }

  return (STATUS_OK);
}

/*
 * this function get the type attribute of the o_timer object.
 */

t_status	interface_timer_attribute_type(o_syscall*	message)
{
  o_timer*		o;

  if (timer_get(message->u.request.u.timer_attribute_type.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.timer_attribute_type.result1 = o->type;
    }

  return (STATUS_OK);
}

/*
 * this function get the handler attribute of the o_timer object.
 */

t_status	interface_timer_attribute_handler(o_syscall*	message)
{
  o_timer*		o;

  if (timer_get(message->u.request.u.timer_attribute_handler.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.timer_attribute_handler.result1 = o->handler;
    }

  return (STATUS_OK);
}

/*
 * this function get the data attribute of the o_timer object.
 */

t_status	interface_timer_attribute_data(o_syscall*	message)
{
  o_timer*		o;

  if (timer_get(message->u.request.u.timer_attribute_data.arg1, &o) != STATUS_OK)
    {
      message->u.reply.error = STATUS_ERROR;
    }
  else
    {
      message->u.reply.error = STATUS_OK;
      message->u.reply.u.timer_attribute_data.result1 = o->data;
    }

  return (STATUS_OK);
}

/*
 * this structure dispatchs incoming system calls.
 */

t_interface_dispatch dispatch[] =
{
  interface_as_copy,
  interface_as_release,
  interface_as_attribute_task,
  interface_event_reserve,
  interface_event_release,
  interface_event_attribute_type,
  interface_event_attribute_handler,
  interface_event_attribute_data,
  interface_map_reserve,
  interface_map_release,
  interface_map_resize,
  interface_region_reserve,
  interface_region_release,
  interface_scheduler_quantum,
  interface_scheduler_yield,
  interface_scheduler_update,
  interface_segment_clone,
  interface_segment_give,
  interface_segment_copy,
  interface_segment_release,
  interface_segment_permissions,
  interface_segment_attribute_as,
  interface_segment_attribute_address,
  interface_segment_attribute_size,
  interface_segment_attribute_permissions,
  interface_task_reserve,
  interface_task_release,
  interface_task_priority,
  interface_task_attribute_parent,
  interface_task_attribute_class,
  interface_task_attribute_behaviour,
  interface_task_attribute_priority,
  interface_task_attribute_as,
  interface_task_attribute_sched,
  interface_thread_release,
  interface_thread_priority,
  interface_thread_load,
  interface_thread_store,
  interface_thread_attribute_task,
  interface_thread_attribute_priority,
  interface_thread_attribute_state,
  interface_timer_reserve,
  interface_timer_release,
  interface_timer_attribute_delay,
  interface_timer_attribute_repeat,
  interface_timer_attribute_type,
  interface_timer_attribute_handler,
  interface_timer_attribute_data,
};

/*
 * this function receives, dispatch and reply incoming system calls.
 *
 * steps:
 *
 * 1) check for syscall correctness.
 * 2) execute the system call.
 * 3) send the reply.
 */

t_status		interface_notify(t_uint8*		buffer,
					 t_vsize		size,
					 i_node			source)
{
  o_syscall*		message = (o_syscall*)buffer;

  /*
   * 1)
   */

  /*  if (size < sizeof (o_syscall))
      return (STATUS_ERROR); */

  if (message->u.request.operation >= INTERFACE_NSYSCALLS)
    return (STATUS_ERROR);

  /*
   * 2)
   */
  t_uint32 op =  message->u.request.operation;
  (void) op; /* UNUSED VAR */

  if (dispatch[message->u.request.operation](message) != STATUS_OK)
    return (STATUS_ERROR);

  /*
   * 3)
   */

  if (message_send(_kernel.task,
		   source,
		   MESSAGE_TYPE_INTERFACE,
		   (t_vaddr)message,
		   sizeof (o_syscall)) != STATUS_OK)
    return (STATUS_ERROR);

  return (STATUS_OK);
}

/*
 * this function initialises the interface manager.
 *
 */

t_status		interface_initialize(void)
{
  if (message_register(_kernel.task, 0, sizeof (o_syscall)) != STATUS_OK)
    return (STATUS_ERROR);

  return (STATUS_OK);
}

/*
 * this function cleans the interface manager.
 */

t_status		interface_clean(void)
{
  return (STATUS_OK);
}
