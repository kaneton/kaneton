/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/kaneton/event/event.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       renaud voltz   [sun feb 12 23:04:54 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the event manager.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX EVENT
 */

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
 */

t_error			event_show(t_eventid			eventid)
{
  t_state		state;
  t_tskid		tsk;
  t_iterator		i;
  o_event*		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (event_get(eventid, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  cons_msg('#', "  event %qd:\n", eventid);

  /*
   * 2)
   */

  cons_msg('#', "    tasks %qd:\n", o->tasks);

  set_foreach(SET_OPT_FORWARD, o->tasks, &i, state)
    {
      if (set_object(o->tasks, i, (void**)&tsk) != ERROR_NONE)
        EVENT_LEAVE(event, ERROR_UNKNOWN);

      cons_msg('#', "      %qd\n", tsk);
    }

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * this function dumps the events managed by the kernel.
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

  if (set_size(event->container, &size) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu event(s):\n", size);

  set_foreach(SET_OPT_FORWARD, event->container, &i, state)
    {
      if (set_object(event->container, i, (void**)&data) != ERROR_NONE)
	EVENT_LEAVE(event, ERROR_UNKNOWN);

      if (event_show(data->eventid) != ERROR_NONE)
	EVENT_LEAVE(event, ERROR_UNKNOWN);
    }

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * notify every subscribed task that an event occured.
 *
 * 1) XXX EVENT timer interrupt : call the timer.
 * 2) XXX EVENT keyboard interrupt : call the keyboard driver.
 * 3) otherwise, message every task.
 */

t_error			event_notify(t_eventid			eventid)
{
  t_state               state;
  t_tskid               tsk;
  t_iterator            i;
  o_event*              o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (eventid == 32)
    {
      /*      printf("timer\n");*/
      timer_handler();
      EVENT_LEAVE(event, ERROR_NONE);
    }

  /*
   * 2)
   */

  if (eventid == 33)
    {
      printf("keyboard event\n");
      EVENT_LEAVE(event, ERROR_NONE);
    }

  /*
   * 3)
   */

  if (event_get(eventid, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  set_foreach(SET_OPT_FORWARD, o->tasks, &i, state)
    {
      if (set_object(o->tasks, i, (void**)&tsk) != ERROR_NONE)
        EVENT_LEAVE(event, ERROR_UNKNOWN);

      /* XXX EVENT
       * send a msg instead of printing it ... */
      printf("event_notify: eventid =%u , tskid=%u\n", eventid, tsk);
    }

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * reserve an event.
 *
 * steps:
 *
 * 1) create a new event object and give it its arch dependent eventid.
 * 2) reserve the set of tskid's.
 * 3) add the new event to the event manager.
 * 4) call the machine dependent code.
 */

t_error			event_reserve(t_eventid			eventid)
{
  o_event		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_event));

  o.eventid = eventid;

  /*
   * 2
   */

  if (set_reserve(ll, SET_OPT_SORT | SET_OPT_ALLOC, sizeof(t_tskid), &o.tasks)
      != ERROR_NONE)
    {
      EVENT_LEAVE(event, ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (set_add(event->container, &o) != ERROR_NONE)
    {
      set_release(o.tasks);

      EVENT_LEAVE(event, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (machdep_call(event, event_reserve, eventid) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * this function releases an event.
 *
 * steps:
 *
 * 1) call the machine dependent code.
 * 2) get the event object from its identifier.
 * 3) release the set of tskid.
 * 4) remove the object from the event container.
 */

t_error			event_release(t_eventid			eventid)
{
  o_event*		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (machdep_call(event, event_release, eventid))
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (event_get(eventid, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (set_release(o->tasks) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_remove(event->container, o->eventid) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * assign a task to an event.
 *
 * steps:
 *
 * 1) get the event object.
 * 2) add a task to this object.
 */

t_error			event_subscribe(t_eventid		eventid,
					t_tskid			tskid)
{
  o_event*		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (event_get(eventid, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_add(o->tasks, &tskid) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * unassociate a task from an event.
 *
 * steps:
 *
 * 1) get the event object.
 * 2) remove the task from this object.
 */

t_error			event_unsubscribe(t_eventid		eventid,
					  t_tskid		tskid)
{
  o_event*		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  if (event_get(eventid, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_remove(o->tasks, tskid) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * this function get an event object from the event container.
 */

t_error			event_get(t_eventid			eventid,
				  o_event**			o)
{
  EVENT_ENTER(event);

  if (set_get(event->container, eventid, (void**)o) != ERROR_NONE)
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
 * 3) reserve the event container.
 * 4) try to reserve a statistic object.
 * 5) call the machine dependent code.
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
		  sizeof(o_event), &event->container) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to reserve the event container\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 4)
   */

  STATS_RESERVE("event", &event->stats);

  /*
   * 5)
   */

  machdep_call(event, event_init);

  return ERROR_NONE;
}

/*
 * destroy the event manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release the statistics object.
 * 3) release the event container.
 * 4) destroy the identifier object.
 * 5) free the event manager's structure memory.
 */

t_error			event_clean(void)
{
  /*
   * 1)
   */

  machdep_call(event, event_clean);

  /*
   * 2)
   */

  STATS_RELEASE(event->stats);

  /*
   * 3)
   */

  if (set_release(event->container) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to release the event container\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 4)
   */

  if (id_destroy(&event->id) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to destroy the identifier object\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 5)
   */

  free(event);

  return ERROR_NONE;
}

/*                                                                  [cut] k3 */
