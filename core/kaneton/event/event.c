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

  cons_msg('#', "  event %qd: task %qd\n", eventid, o->handler.taskid);

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
 * notify task that an event occured.
 */

t_error			event_notify(t_eventid			eventid)
{
  o_event*              o;

  EVENT_ENTER(event);

  if (event_get(eventid, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * XXX EVENT
   * message the task !
   */

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * reserve an event.
 *
 * steps:
 *
 * 1) create a new event object and give it its arch dependent eventid.
 * 2) add the new event to the event manager.
 * 3) call the machine dependent code.
 */

t_error			event_reserve(t_eventid			eventid,
				      e_event_type		type,
				      u_event_handler		handler)
{
  o_event		o;

  EVENT_ENTER(event);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_event));

  o.eventid = eventid;

  o.type = type;

  o.handler = handler;

  /*
   * 2)
   */

  if (set_add(event->container, &o) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (machdep_call(event, event_reserve, eventid, type, handler) != ERROR_NONE)
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
 * 3) remove the object from the event container.
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

  if (set_remove(event->container, o->eventid) != ERROR_NONE)
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

/*
 * XXX EVENT remove me !
 * just for testing.
 */


static const char       scancodes[] =
  {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', 0, 0, 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';',
    '\'', '`', '-', '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, 0, 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };


void                    kbd_handler(t_uint32                    id)
{
  t_uint8               scancode;

  INB(0x60, scancode);

  if (scancode < 70)
    printf("%c", scancodes[scancode]);
}

void			pf_handler(t_uint32			error_code)
{
  printf ("error_code = %u\n", error_code);

  printf("PAGE FAULT: trying to %s a %s page\n",
	 (error_code & 2) ? "write on" : "read on",
	 (error_code & 1) ? "non authorized" : "non present");

  while (1);
}


t_error			event_test(void)
{
  u_event_handler	handler;

  handler.function = timer_handler;

  if (event_reserve(32, EVENT_FUNCTION, handler) != ERROR_NONE)
    return ERROR_UNKNOWN;

  handler.function = kbd_handler;

  if (event_reserve(33, EVENT_FUNCTION, handler) != ERROR_NONE)
    return ERROR_UNKNOWN;

  handler.function = pf_handler;

  if (event_reserve(14, EVENT_FUNCTION, handler) != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_UNKNOWN;
}

/*                                                                  [cut] k3 */
