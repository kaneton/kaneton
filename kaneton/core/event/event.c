/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/event/event.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:26:00 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the event manager.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(event);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager variable.
 */

m_event*                 event = NULL;

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel*	kernel;

/*
 * the kernel task identifier.
 */

extern i_task		ktask;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given event.
 *
 * steps:
 *
 * 1) get the event object from its identifier.
 * 2) display the tasks which have subscribed to this event.
 * 3) call the machine0dependent code.
 */

t_error			event_show(i_event			id)
{
  o_event*		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (event_get(id, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "  event %qd: task %qd\n", o->eventid, o->handler.taskid);

  /*
   * 3)
   */

  if (machine_call(event, event_show, id) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * this function dumps the events.
 *
 * steps:
 *
 * 1) get the set size.
 * 2) show each event.
 */

t_error			event_dump(void)
{
  t_state		state;
  o_event*		data;
  t_setsz		size;
  t_iterator		i;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (set_size(event->events, &size) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu event(s):\n", size);

  set_foreach(SET_OPT_FORWARD, event->events, &i, state)
    {
      if (set_object(event->events, i, (void**)&data) != ERROR_NONE)
	EVENT_LEAVE(event, ERROR_UNKNOWN);

      if (event_show(data->eventid) != ERROR_NONE)
	EVENT_LEAVE(event, ERROR_UNKNOWN);
    }

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * notify a task that an event occured.
 *
 * steps:
 *
 * 1) get the event object from the set.
 * 2) notify the task its event has occured.
 * 3) call machdep.
 */

t_error			event_notify(i_event			id)
{
  o_event*              o;
  o_event_message	msg;
  i_node		node;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (event_get(id, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  msg.id = id;
  msg.data = o->data;
  node.machine = kernel->machine;
  node.task = o->handler.taskid;

  if (message_send(ktask, node, MESSAGE_TYPE_EVENT, (t_vaddr)&msg,
		   sizeof (o_event_message)) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (machine_call(event, event_notify, id) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * reserve an event.
 *
 * steps:
 *
 * 1) check that the event has not been already reserved.
 * 2) create a new event object and give it its arch dependent eventid.
 * 3) add the new event to the event manager.
 * 4) call the machine dependent code.
 */

t_error			event_reserve(i_event			id,
				      t_type			type,
				      u_event_handler		handler,
				      t_vaddr			data)
{
  o_event*		tmp;
  o_event		o;

  EVENT_ENTER(event);

  ASSERT(type == EVENT_FUNCTION || type == EVENT_MESSAGE);

  /*
   * 1)
   */

  if (event_get(id, &tmp) == ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_event));

  o.eventid = id;

  o.type = type;

  o.handler = handler;
  o.data = data;

  /*
   * 3)
   */

  if (set_add(event->events, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machine_call(event, event_reserve, id, type, handler,
		   data) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * this function releases an event.
 *
 * steps:
 *
 * 1) get the event object from its identifier.
 * 2) remove the object from the event set.
 * 3) call the machine dependent code.
 */

t_error			event_release(i_event			id)
{
  o_event*		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (event_get(id, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_remove(event->events, o->eventid) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (machine_call(event, event_release, o->eventid) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * this function gets an event object from the event set.
 */

t_error			event_get(i_event			id,
				  o_event**			o)
{
  EVENT_ENTER(event);

  ASSERT(o != NULL);

  if (set_get(event->events, id, (void**)o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * initialize the event manager.
 *
 * steps:
 *
 * 1) allocate and initialize the event manager.
 * 2) initialize the object identifier.
 * 3) reserve the event set.
 * 4) call the machine dependent code.
 */

t_error			event_initialize(void)
{
  /*
   * 1)
   */

  if ((event = malloc(sizeof(m_event))) == NULL)
    {
      cons_msg('!', "event: cannot allocate memory for the event manager "
	       "structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(event, 0x0, sizeof(m_event));

  /*
   * 2)
   */

  if (id_build(&event->id) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to initialize the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_event), &event->events) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to reserve the event set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (machine_call(event, event_initialize) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * destroy the event manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release every event object.
 * 3) release the event set.
 * 4) destroy the identifier object.
 * 5) free the event manager's structure memory.
 */

t_error			event_clean(void)
{
  t_iterator		i;
  o_event*		o;

  /*
   * 1)
   */

  if (machine_call(event, event_clean) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  while (set_head(event->events, &i) == ERROR_NONE)
    {
      if (set_object(event->events, i, (void**)&o) != ERROR_NONE)
	{
	  cons_msg('!', "event: cannot find the event object "
		   "corresponding to its identifier\n");

	  return (ERROR_UNKNOWN);
	}

      if (event_release(o->eventid) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (set_release(event->events) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to release the event set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (id_destroy(&event->id) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  free(event);

  return (ERROR_NONE);
}
