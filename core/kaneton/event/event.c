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
 * XXX EVENT
 */

t_error			event_show(t_eventid			eventid)
{
  EVENT_ENTER(event);



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
   * 2
   */

  cons_msg('#', "dumping %qu event(s):\n", size);

  set_foreach(SET_OPT_FORWARD, event->container, &i, state)
    {
      if (set_object(event->container, i, (void**)&data) != ERROR_NONE)
	EVENT_LEAVE(event, ERROR_UNKNOWN);

      if (task_show(data->eventid) != ERROR_NONE)
	EVENT_LEAVE(event, ERROR_UNKNOWN);
    }

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * XXX EVENT
 */

t_error			event_subscribe(t_eventid		evenid,
					t_tskid			tskid)
{
  EVENT_ENTER(event);



  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * XXX EVENT
 */
t_error			event_unsubscribe(t_eventid		evenid,
					  t_tskid		taskid)
{
  EVENT_ENTER(event);



  EVENT_LEAVE(event, ERROR_NONE);
}

t_error			event_init(void)
{

  if ((event = malloc(sizeof(m_event))) == NULL)
    {
      cons_msg('!', "event: cannot allocate memory for the event manager "
	       "structure\n");

      return ERROR_UNKNOWN;
    }

  memset(event, 0x0, sizeof(m_event));

  /*
   *
   */

  if (id_build(&event->id) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to initialize the identifier object\n");

      return ERROR_UNKNOWN;
    }

  /*
   *
   */

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_event), &event->container) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to reserve the event container\n");

      return ERROR_UNKNOWN;
    }

  /*
   *
   */

  STATS_RESERVE("event", &event->stats);

  /*
   *
   */

  machdep_call(event, event_init);

  return ERROR_NONE;
}


t_error			event_clean(void)
{
  /*
   *
   */


  /*
   *
   */

  STATS_RELEASE(event->stats);

  /*
   *
   */

  if (set_release(event->container) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to release the event container\n");

      return ERROR_UNKNOWN;
    }

  /*
   *
   */

  if (id_destroy(&event->id) != ERROR_NONE)
    {
      cons_msg('!', "event: unable to destroy the identifier object\n");

      return ERROR_UNKNOWN;
    }

  /*
   *
   */

  free(event);

  return ERROR_NONE;
}

/*                                                                  [cut] k3 */
