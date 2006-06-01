/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/event/event.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       matthieu bucchianeri   [thu jun  1 18:07:11 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the event manager.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX EVENT assignments have to be written.
 */

/*                                                                  [cut] k3 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(event);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager variable.
 */

m_event*                 event = NULL;

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

  if (machdep_call(event, event_show, id) != ERROR_NONE)
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
 */

t_error			event_notify(i_event			id)
{
  o_event*              o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (event_get(id, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  /* XXX EVENT message the task ! */

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * reserve an event.
 *
 * steps:
 *
 * 1) check that the event has not been already reserved.
 * 2) call the machine dependent code.
 * 3) create a new event object and give it its arch dependent eventid.
 * 4) add the new event to the event manager.
 */

t_error			event_reserve(i_event			id,
				      t_type			type,
				      u_event_handler		handler)
{
  o_event*		tmp;
  o_event		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (event_get(id, &tmp) == ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machdep_call(event, event_reserve, id, type, handler) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memset(&o, 0x0, sizeof(o_event));

  o.eventid = id;

  o.type = type;

  o.handler = handler;

  /*
   * 4)
   */

  if (set_add(event->events, &o) != ERROR_NONE)
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

  if (machdep_call(event, event_release, o->eventid) != ERROR_NONE)
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
 * 4) try to reserve a statistic object.
 * 5) call the machine dependent code.
 * 6) dump the event manager if debug is enabled.
 */

t_error			event_init(void)
{
  /*
   * 1)
   */

  if ((event = malloc(sizeof(m_event))) == NULL)
    {
      cons_msg('!', "event: cannot allocate memory for the event manager "
	       "structure\n");

      return ERROR_UNKNOWN;
    }

  memset(event, 0x0, sizeof(m_event));

  /*
   * 2)
   */

  if (id_build(&event->id) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to initialize the identifier object\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_event), &event->events) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to reserve the event set\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 4)
   */

  STATS_RESERVE("event", &event->stats);

  /*
   * 5)
   */

  if (machdep_call(event, event_init) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 6)
   */

#if (DEBUG & DEBUG_EVENT)
  if (event_dump() != ERROR_NONE)
    return ERROR_UNKNOWN;
#endif

  return ERROR_NONE;
}

/*
 * destroy the event manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release the statistics object.
 * 3) release every event object.
 * 4) release the event set.
 * 5) destroy the identifier object.
 * 6) free the event manager's structure memory.
 */

t_error			event_clean(void)
{
  t_iterator		i;
  o_event*		o;

  /*
   * 1)
   */

  if (machdep_call(event, event_clean) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  STATS_RELEASE(event->stats);

  /*
   * 3)
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
   * 4)
   */

  if (set_release(event->events) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to release the event set\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 5)
   */

  if (id_destroy(&event->id) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to destroy the identifier object\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 6)
   */

  free(event);

  return ERROR_NONE;
}

/*
 * XXX EVENT remove me !
 * just for testing.
 */

t_error			event_test(void)
{
  if (event_reserve(32, EVENT_FUNCTION, (u_event_handler)timer_handler)
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (event_reserve(33, EVENT_FUNCTION, (u_event_handler)ia32_kbd_handler)
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (event_reserve(14, EVENT_FUNCTION, (u_event_handler)ia32_pf_handler)
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_UNKNOWN;
}

/*                                                                 [cut] /k3 */
