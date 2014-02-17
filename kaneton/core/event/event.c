/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/event/event.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       julien quintard   [wed mar  2 20:26:12 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the event managers provides the possibility to generate a notification
 * every time a given hardware event occurs.
 *
 * note that the event identifiers are machine-specific. therefore, such
 * events should not be reserved from the core since such events may make
 * no sense on another machine.
 *
 * the notification can be delivered in two forms. either a function can be
 * directly called, form which is especially useful for the kernel or a
 * message can be sent to a task. this last form of notification is interesting
 * should a server be interested in receiving specific hardware events. for
 * example, the ATA server may be interested in being notified of every
 * hardware interrupt related to the ATA devices.
 *
 * [XXX:improvements] provide the possibility for multiple handlers on a
 *                    the same event.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(event);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager.
 */

m_event			_event;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given event's attributes.
 *
 * steps:
 *
 * 1) retrieve the event object.
 * 2) display the event's attributes depending on the type of notification.
 * 3) call the machine.
 */

t_status		event_show(i_event			id,
				   mt_margin			margin)
{
  o_event*		o;

  /*
   * 1)
   */

  if (event_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the event object");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "event:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  switch (o->type)
    {
    case EVENT_TYPE_FUNCTION:
      {
	module_call(console, message,
		    '#',
		    MODULE_CONSOLE_MARGIN_FORMAT
		    "  core: id(%qd) type(function) routine(0x%x)\n",
		    MODULE_CONSOLE_MARGIN_VALUE(margin),
		    o->id,
		    o->handler.routine);

	break;
      }
    case EVENT_TYPE_MESSAGE:
      {
	module_call(console, message,
		    '#',
		    MODULE_CONSOLE_MARGIN_FORMAT
		    "  core: id(%qd) type(message) task(%qd)\n",
		    MODULE_CONSOLE_MARGIN_VALUE(margin),
		    o->id,
		    o->handler.task);

	break;
      }
    default:
      CORE_ESCAPE("unknown event type '%u'",
		  o->type);
    }

  /*
   * 3)
   */

  if (machine_call(event, show, id, margin) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the event manager.
 *
 * steps:
 *
 * 1) display general information.
 * 2) retrieve the number of registered events.
 * 3) show every event.
 * 4) call the machine.
 */

t_status		event_dump(void)
{
  o_event*		data;
  t_setsz		size;
  t_state		state;
  s_iterator		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "event manager:\n",
	      _event.events);

  module_call(console, message,
	      '#', "  core: events(%qd)\n",
	      _event.events);

  /*
   * 2)
   */

  if (set_size(_event.events, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of events");

  /*
   * 3)
   */

  module_call(console, message,
	      '#', "    events: id(%qd) size(%qd)\n",
	      _event.events,
	      size);

  set_foreach(SET_OPTION_FORWARD, _event.events, &i, state)
    {
      if (set_object(_event.events, i, (void**)&data) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the object from the set of events");

      if (event_show(data->id,
		     3 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to show the object");
    }

  /*
   * 4)
   */

  if (machine_call(event, dump) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * notify a task that an event occured.
 *
 * steps:
 *
 * 1) retrieve the event object.
 * 2) notify the target of the event depending on the type of notification
 *    set up at the reservation.
 *   a) call the routine, taking to pass both the event identifier and
 *      the data provided at the reservation.
 *   b) send a message to the target task
 *   c) return an error since the type of notification is not supported.
 * 3) call the machine.
 */

t_status		event_notify(i_event			id)
{
  o_event*              o;

  /*
   * 1)
   */

  if (event_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the event object");

  /*
   * 2)
   */

  switch (o->type)
    {
      /*
       * a)
       */

    case EVENT_TYPE_FUNCTION:
      {
	o->handler.routine(o->id, o->data);

	break;
      }

      /*
       * b)
       */

    case EVENT_TYPE_MESSAGE:
      {
	o_event_message	message;
	i_node		node;

	message.id = id;
	message.data = o->data;

	node.cell = _kernel.cell;
	node.task = o->handler.task;

	if (message_send(_kernel.task,
			 node,
			 MESSAGE_TYPE_EVENT,
			 (t_vaddr)&message,
			 sizeof (o_event_message)) != STATUS_OK)
	  CORE_ESCAPE("unable to send a message to the destination task");

	break;
      }

      /*
       * c)
       */

    default:
      CORE_ESCAPE("unknown event type '%u'",
		  o->type);
    }

  /*
   * 3)
   */

  if (machine_call(event, notify, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function activates the event processing. note that this operation
 * is left to the machine.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_status		event_enable(void)
{
  /*
   * 1)
   */

  if (machine_call(event, enable) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function deactivates the event processing. note that this operation
 * is left to the machine.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_status		event_disable(void)
{
  /*
   * 1)
   */

  if (machine_call(event, disable) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reserves an event that must be notified by the given
 * method: intra-kernel function call or via a message.
 *
 * note that the macro-functions EVENT_TYPE_FUNCTION/EVENT_TYPE_MESSAGE
 * and EVENT_ROUTINE()/EVENT_TASK() are provide to make reservation
 * easier.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) check that the event has not been already reserved.
 * 2) create and initialize a new event object.
 * 3) add the object to the set of events.
 * 4) call the machine.
 */

t_status		event_reserve(i_event			id,
				      t_type			type,
				      u_event_handler		handler,
				      t_data			data)
{
  o_event		o;

  /*
   * 0)
   */

  if ((type != EVENT_TYPE_FUNCTION) &&
      (type != EVENT_TYPE_MESSAGE))
    CORE_ESCAPE("unknown event type '%u'",
		type);

  /*
   * 1)
   */

  if (event_exist(id) == TRUE)
    CORE_ESCAPE("this event has already been reserved");

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof (o_event));

  o.id = id;
  o.type = type;
  o.handler = handler;
  o.data = data;

  /*
   * 3)
   */

  if (set_add(_event.events, &o) != STATUS_OK)
    CORE_ESCAPE("unable to add the object to the set of events");

  /*
   * 4)
   */

  if (machine_call(event, reserve, id, type, handler, data) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases an event.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) remove the object from the set of events.
 */

t_status		event_release(i_event			id)
{
  /*
   * 1)
   */

  if (machine_call(event, release, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (set_remove(_event.events, id) != STATUS_OK)
    CORE_ESCAPE("unable to remove the object from the set of events");

  CORE_LEAVE();
}

/*
 * this function returns true if the event exists i.e has been reserved.
 */

t_bool			event_exist(i_event			id)
{
  if (set_exist(_event.events, id) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function retrieves the object from the set of events.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the object from the set.
 */

t_status		event_get(i_event			id,
				  o_event**			object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_get(_event.events, id, (void**)object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the event object from the set");

  CORE_LEAVE();
}

/*
 * this function initializes the event manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the event manager's structure.
 * 3) reserve the set of events.
 * 4) call the machine.
 */

t_status		event_initialize(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the event manager\n");

  /*
   * 2)
   */

  memset(&_event, 0x0, sizeof (m_event));

  /*
   * 3)
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOCATE | SET_OPTION_SORT,
		  sizeof (o_event),
		  &_event.events) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set of events");

  /*
   * 4)
   */

  if (machine_call(event, initialize) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the event manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 * 3) release every event object.
 * 4) release the set of events.
 */

t_status		event_clean(void)
{
  s_iterator		i;
  o_event*		o;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the event manager\n");

  /*
   * 2)
   */

  if (machine_call(event, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  while (set_head(_event.events, &i) == TRUE)
    {
      if (set_object(_event.events, i, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to find the event object corresponding to "
		    "its identifier");

      if (event_release(o->id) != STATUS_OK)
	CORE_ESCAPE("unable to release the event object");
    }

  /*
   * 4)
   */

  if (set_release(_event.events) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of events");

  CORE_LEAVE();
}
