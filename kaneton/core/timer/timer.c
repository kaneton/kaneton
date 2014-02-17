/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/timer/timer.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       julien quintard   [wed mar  2 20:27:23 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the timer manager provides functionalities for triggering functions, or
 * sending message notifications, after some time has passed.
 *
 * note that timer objects are kept in an ordered list, with the
 * about-to-be-triggered timers first. therefore, whenever the manager
 * wants to check whether some timers have expired, it just checks if
 * the object's delay has expired. if so the timer's expiration is notified
 * while the next timer is checked until a timer is detected as not having
 * expired in which case, since the timers are ordered according to their
 * delay, all the following timers do not need to be checked.
 *
 * also, the timer's delay are relative to each other. for instance considering
 * the following set of delays (15, 20, 28, 38, 40, 40 and 100) would be
 * stored in the sorted set as follows:
 *
 *   15, 5, 8, 10, 2, 0, 60
 *
 * note that the first timer keeps its absolute value while the others
 * are relative to the previous one.
 *
 * since timers can have the same delay, mapping the timer identifier on
 * the delay would incurr identifier collisions. an identifier is thus
 * generated. this implies that the sorting cannot be left to the set
 * manager since the identifiers sorting does not reflect the timers
 * sorting. therefore, the manager itself handles both the objects' memory
 * and their placement in the list.
 *
 * in addition, note that a timer can be either internal or external. the
 * first type is associated with an internal kernel function which is called
 * on expiration while the second generates a message which is sent to the
 * destination task.
 *
 * finally, timers can be configured, through the REPEAT option, in order
 * to be triggered on a regular basis, every 'delay' milliseconds. therefore,
 * once such a timer has expired, its delay is re-initialised to its original
 * value and the timer is re-located in the list, according to its new delay.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(timer);

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
 * the timer manager.
 */

m_timer			_timer;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function is triggered by the hardware timer event on a regular
 * basis.
 *
 * note that since the hardware timer event is, obviously, machine-dependent,
 * it is the machine's reponsability to reserve this event so that this
 * function gets triggered.
 *
 * steps:
 *
 * 1) update the clock.
 * 2) check the timers' expiration.
 */

void			timer_handler(i_event			event,
				      t_data			data)
{
  /*
   * 1)
   */

  assert(clock_update(TIMER_DELAY) == STATUS_OK);

  /*
   * 2)
   */

  assert(timer_check() == STATUS_OK);
}

/*
 * this function is called on a regular basis and check whether the
 * timers have expired or not. if so, the expiration is notified to its
 * associated target.
 *
 * steps:
 *
 * 1) go through the timers.
 *   a) retrieve the timer object.
 *   b) if the timer still needs to wait to be triggered, decrease it by
 *      the amount of time that has passed and exit the loop since, if
 *      this timer has not expired, none of the following has.
 *   c) since this timer has expired, substract its delay to the amount
 *      of time that needs to be substracted from all the expiring timers.
 *   d) update the timer's delay to zero since it has expired.
 *   e) notify of the timer's expiration.
 *   f) if the timer must be repeated, update it so that it gets re-inserted
 *      in the list. otherwise, release the timer since no longer used.
 *   g) finally move on to the next timer which may also have expired.
 */

t_status		timer_check(void)
{
  t_delay		reference = TIMER_DELAY;
  s_iterator		i;

  /*
   * 1)
   */

  while (set_head(_timer.timers, &i) == TRUE)
    {
      o_timer*		o;

      /*
       * a)
       */

      if (set_object(_timer.timers, i, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the timer");

      /*
       * b)
       */

      if (o->delay > reference)
	{
	  o->delay -= reference;

	  break;
	}

      /*
       * c)
       */

      reference -= o->delay;

      /*
       * d)
       */

      o->delay = 0;

      /*
       * e)
       */

      if (timer_notify(o->id) != STATUS_OK)
	CORE_ESCAPE("unable to notify of the timer's expiration");

      /*
       * f)
       */

      if (o->options & TIMER_OPTION_REPEAT)
        {
	  if (timer_update(o->id, o->repeat) != STATUS_OK)
	    CORE_ESCAPE("unable to update the timer");
        }
      else
        {
          if (timer_release(o->id) != STATUS_OK)
	    CORE_ESCAPE("unable to release the timer");
        }

      /*
       * g)
       */
    }

  CORE_LEAVE();
}

/*
 * this function shows a timer's attributes.
 *
 * steps:
 *
 * 1) retrieve the timer object.
 * 2) build the options string.
 * 3) depending on the timer type, display its attributes.
 * 4) call the machine.
 */

t_status		timer_show(i_timer			id,
				   mt_margin			margin)
{
  o_timer*		o;
  char			options[2];

  /*
   * 1)
   */

  if (timer_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 2)
   */

  if (o->options & TIMER_OPTION_REPEAT)
    options[0] = 'r';
  else
    options[0] = '.';

  options[1] = '\0';

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "timer:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  switch (o->type)
    {
    case TIMER_TYPE_FUNCTION:
      {
	module_call(console, message,
		    '#',
		    MODULE_CONSOLE_MARGIN_FORMAT
		    "  core: id(%qd) type(function) delay(%qd) options(%s) "
		    "repeat(%qd) routine(0x%x) data(0x%x)\n",
		    MODULE_CONSOLE_MARGIN_VALUE(margin),
		    o->id,
		    o->delay,
		    options,
		    o->repeat,
		    o->handler.routine,
		    o->data);

	break;
      }
    case TIMER_TYPE_MESSAGE:
      {
	module_call(console, message,
		    '#',
		    MODULE_CONSOLE_MARGIN_FORMAT
		    "  core: id(%qd) type(message) delay(%qd) options(%s) "
		    "repeat(%qd) task(%qd) data(0x%x)\n",
		    MODULE_CONSOLE_MARGIN_VALUE(margin),
		    o->id,
		    o->delay,
		    options,
		    o->repeat,
		    o->handler.task,
		    o->data);

	break;
      }
    default:
      CORE_ESCAPE("unknown timer type '%u'",
		  o->type);
    }

  /*
   * 4)
   */

  if (machine_call(timer, show, id, margin) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the timer manager.
 *
 * steps:
 *
 * 1) display general information on the manager.
 * 2) show the identifier object.
 * 3) retrieve the size of the set of timers.
 * 4) display the timers.
 *   a) retrieve the timer object.
 *   b) show the timer.
 * 5) call the machine.
 */

t_status		timer_dump(void)
{
  t_setsz		size;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "timer manager:\n");

  module_call(console, message,
	      '#', "  core: timers(%qd)\n",
	      _timer.timers);

  /*
   * 2)
   */

  if (id_show(&_timer.id,
	      2 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
    CORE_ESCAPE("unable to show the identifier object");

  /*
   * 3)
   */

  if (set_size(_timer.timers, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of timers");

  /*
   * 4)
   */

  module_call(console, message,
	      '#', "    timers: id(%qd) size(%qd)\n",
	      _timer.timers,
	      size);

  set_foreach(SET_OPTION_FORWARD, _timer.timers, &i, s)
    {
      o_timer*		o;

      /*
       * a)
       */

      if (set_object(_timer.timers, i, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the timer");

      /*
       * b)
       */

      if (timer_show(o->id,
		     3 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to show the timer");
    }

  /*
   * 5)
   */

  if (machine_call(timer, dump) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function notifies the destination, i.e depending on the timer type,
 * of the timer's expiration.
 *
 * steps:
 *
 * 1) retrieve the timer object.
 * 2) depending on the timer type.
 *   A) if the timer target is a function...
 *     a) call the associated routine.
 *   B) if the timer target is a message...
 *     a) build the destination node.
 *     b) build the notification message.
 *     c) send the message.
 * 3) call the machine.
 */

t_status		timer_notify(i_timer			id)
{
  o_timer*		o;

  /*
   * 1)
   */

  if (timer_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 2)
   */

  switch (o->type)
    {
    case TIMER_TYPE_FUNCTION:
      {
	/*
	 * A)
	 */

	/*
	 * a)
	 */

	o->handler.routine(o->id, o->data);

	break;
      }
    case TIMER_TYPE_MESSAGE:
      {
	/*
	 * B)
	 */

	o_timer_message	message;
	i_node		node;

	/*
	 * a)
	 */

	node.cell = _kernel.cell;
	node.task = o->handler.task;

	/*
	 * b)
	 */

	message.id = id;
	message.data = o->data;

	/*
	 * c)
	 */

	if (message_send(_kernel.task,
			 node,
			 MESSAGE_TYPE_TIMER,
			 (t_vaddr)&message,
			 sizeof (o_timer_message)) != STATUS_OK)
	  CORE_ESCAPE("unable to send a message to the task");
      }
    default:
      CORE_ESCAPE("unknown timer type '%u'",
		  o->type);
    }

  /*
   * 3)
   */

  if (machine_call(timer, notify, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reserves a timer.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) reserve a timer identifier.
 * 2) allocate memory for the timer object.
 * 3) initialize and fill the object.
 * 4) if the repeat option has been activated, save the original delay.
 * 5) insert the object in the set, according to its state:
 *   A) if the set is empty...
 *     a) since this is the only timer, set the relative delay to the
 *        absolute's size.
 *     b) add the timer.
 *   B) if there already is a timer, or more...
 *     a) initialize the absolute reference delay.
 *     b) go through the timers.
 *       i) retrieve the timer object.
 *       ii) add the timer's relative delay to the absolute delay.
 *       iii) if the absolute delay of the reserved timer is larger than
 *            the absolute delay of the timer being handled in the loop, this
 *            means that the timer must be stored further in the set; continue
 *            with the next timer.
 *       iv) compute the reserved timer's delay depending on its position i.e
 *           first or following another timer.
 *         #1) if the timer is the first one, the object's relative delay
 *             equals the absolute one.
 *         #2) if the timer is preceded by another time, the absolute value
 *             given through timer_reserve() must be converted in a delay
 *             relative to the previous timer's: difference between the
 *             absolute delays of the reserved and previous timers.
 *       v) update the current timer since its relative delay must be adjusted
 *          to the reserved timer which will soon precede it.
 *       vi) insert the new timer before the current one.
 *       vii) the timer has been inserted properly, exit the loop.
 *     c) if the function reaches this point, no timer with a higher delay
 *        was found. therefore, set the object's relative delay as being
 *        the absolute provided through timer_reserve() minus the sum
 *        of all the timers i.e _reference_.
 *     d) add the timer after the last timer i.e at the end.
 * 6) call the machine.
 */

t_status		timer_reserve(t_type			type,
				      u_timer_handler		handler,
				      t_data			data,
				      t_delay			delay,
				      t_options			options,
				      i_timer*			id)
{
  t_delay		reference;
  o_timer*		o;
  s_iterator		i;
  t_state		s;

  /*
   * 0)
   */

  if ((type != TIMER_TYPE_FUNCTION) &&
      (type != TIMER_TYPE_MESSAGE))
    CORE_ESCAPE("invalid type");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  /*
   * 1)
   */

  if (id_reserve(&_timer.id, id) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a timer identifier");

  /*
   * 2)
   */

  if ((o = malloc(sizeof (o_timer))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the timer object");

  /*
   * 3)
   */

  memset(o, 0x0, sizeof (o_timer));

  o->id = *id;
  o->type = type;
  o->handler = handler;
  o->data = data;
  o->options = options;

  /*
   * 4)
   */

  if (o->options & TIMER_OPTION_REPEAT)
    o->repeat = delay;

  /*
   * 5)
   */

  if (set_empty(_timer.timers) == TRUE)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      o->delay = delay;

      /*
       * b)
       */

      if (set_add(_timer.timers, o) != STATUS_OK)
	CORE_ESCAPE("unable to append the timer object at the end of the set");
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      reference = 0;

      /*
       * b)
       */

      set_foreach(SET_OPTION_FORWARD, _timer.timers, &i, s)
	{
	  o_timer*	n;
	  s_iterator	j;

	  /*
	   * i)
	   */

	  if (set_object(_timer.timers, i, (void**)&n) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the timer");

	  /*
	   * ii)
	   */

	  reference += n->delay;

	  /*
	   * iii)
	   */

	  if (delay >= reference)
	    continue;

	  /*
	   * iv)
	   */

	  if (set_previous(_timer.timers, i, &j) == FALSE)
	    {
	      /*
	       * #1)
	       */

	      o->delay = delay;
	    }
	  else
	    {
	      /*
	       * #2)
	       */

	      o->delay = delay - (reference - n->delay);
	    }

	  /*
	   * v)
	   */

	  n->delay = n->delay - o->delay;

	  /*
	   * vi)
	   */

	  if (set_before(_timer.timers, i, o) != STATUS_OK)
	    CORE_ESCAPE("unable to insert the timer in the set");

	  /*
	   * vii)
	   */

	  goto inserted;
	}

      /*
       * c)
       */

      o->delay = delay - reference;

      /*
       * d)
       */

      if (set_after(_timer.timers, i, o) != STATUS_OK)
	CORE_ESCAPE("unable to insert the timer in the set");
    }

 inserted:

  /*
   * 6)
   */

  if (machine_call(timer, reserve,
		   type,
		   handler,
		   data,
		   delay,
		   options,
		   id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases a timer.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) release the timer identifier.
 * 3) locate and retrieve the timer object.
 * 4) update the following timer's relative delay.
 *   a) retrieve the next timer, if there is one.
 *   b) re-compute its delay since its previous timer just got released.
 * 5) release the object's memory.
 * 6) delete the timer from the list.
 */

t_status		timer_release(i_timer			id)
{
  o_timer*		o;
  s_iterator		i;
  s_iterator		j;

  /*
   * 1)
   */

  if (machine_call(timer, release, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (id_release(&_timer.id, id) != STATUS_OK)
    CORE_ESCAPE("unable to release the timer identifier");

  /*
   * 3)
   */

  if (set_locate(_timer.timers, id, &i) != STATUS_OK)
    CORE_ESCAPE("unable to locate the timer object");

  if (set_object(_timer.timers, i, (void**)&o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 4)
   */

  if (set_next(_timer.timers, i, &j) == TRUE)
    {
      o_timer*		t;

      /*
       * a)
       */

      if (set_object(_timer.timers, j, (void**)&t) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the timer object");

      /*
       * b)
       */

      t->delay += o->delay;
    }

  /*
   * 5)
   */

  free(o);

  /*
   * 6)
   */

  if (set_delete(_timer.timers, i) != STATUS_OK)
    CORE_ESCAPE("unable to delete the object from the set of timers");

  CORE_LEAVE();
}

/*
 * this function updates the timer's delay.
 *
 * note that should the timer be repeated, its repeat delay is also
 * updated.
 *
 * steps:
 *
 * 1) locate and retrieve the timer object.
 * 2) delete the object from the list.
 * 3) if the timer is being repeated, update its repeat delay.
 * 4) insert the object in the set, according to its state:
 *   A) if the set is empty...
 *     a) since this is the only timer, set the relative delay to the
 *        absolute's size.
 *     b) add the timer.
 *   B) if there already is a timer, or more...
 *     a) initialize the absolute reference delay.
 *     b) go through the timers.
 *       i) retrieve the timer object.
 *       ii) add the timer's relative delay to the absolute delay.
 *       iii) if the absolute delay of the updated timer is larger than
 *            the absolute delay of the timer being handled in the loop, this
 *            means that the timer must be stored further in the set; continue
 *            with the next timer.
 *       iv) compute the updated timer's delay depending on its position i.e
 *           first or following another timer.
 *         #1) if the timer is the first one, the object's relative delay
 *             equals the absolute one.
 *         #2) if the timer is preceded by another time, the absolute value
 *             given through timer_update() must be converted in a delay
 *             relative to the previous timer's: difference between the
 *             absolute delays of the updated and previous timers.
 *       v) update the current timer since its relative delay must be adjusted
 *          to the updated timer which will soon precede it.
 *       vi) insert the new timer before the current one.
 *       vii) the timer has been inserted properly, exit the loop.
 *     c) if the function reaches this point, no timer with a higher delay
 *        was found. therefore, set the object's relative delay as being
 *        the absolute provided through timer_update() minus the sum
 *        of all the timers i.e _reference_.
 *     d) add the timer after the last timer i.e at the end.
 * 5) call the machine.
 */

t_status		timer_update(i_timer			id,
				     t_delay			delay)
{
  t_delay		reference;
  o_timer*		o;
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  if (set_locate(_timer.timers, id, &i) != STATUS_OK)
    CORE_ESCAPE("unable to locate the timer object");

  if (set_object(_timer.timers, i, (void**)&o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 2)
   */

  if (set_delete(_timer.timers, i) != STATUS_OK)
    CORE_ESCAPE("unable to delete the object from the set of timers");

  /*
   * 3)
   */

  if (o->options & TIMER_OPTION_REPEAT)
    o->repeat = delay;

  /*
   * 4)
   */

  if (set_empty(_timer.timers) == TRUE)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      o->delay = delay;

      /*
       * b)
       */

      if (set_add(_timer.timers, o) != STATUS_OK)
	CORE_ESCAPE("unable to append the timer object at the end of the set");
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      reference = 0;

      /*
       * b)
       */

      set_foreach(SET_OPTION_FORWARD, _timer.timers, &i, s)
	{
	  o_timer*	n;
	  s_iterator	j;

	  /*
	   * i)
	   */

	  if (set_object(_timer.timers, i, (void**)&n) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the timer");

	  /*
	   * ii)
	   */

	  reference += n->delay;

	  /*
	   * iii)
	   */

	  if (delay >= reference)
	    continue;

	  /*
	   * iv)
	   */

	  if (set_previous(_timer.timers, i, &j) == FALSE)
	    {
	      /*
	       * #1)
	       */

	      o->delay = delay;
	    }
	  else
	    {
	      /*
	       * #2)
	       */

	      o->delay = delay - (reference - n->delay);
	    }

	  /*
	   * v)
	   */

	  n->delay = n->delay - o->delay;

	  /*
	   * vi)
	   */

	  if (set_before(_timer.timers, i, o) != STATUS_OK)
	    CORE_ESCAPE("unable to insert the timer in the set");

	  /*
	   * vii)
	   */

	  goto inserted;
	}

      /*
       * c)
       */

      o->delay = delay - reference;

      /*
       * d)
       */

      if (set_after(_timer.timers, i, o) != STATUS_OK)
	CORE_ESCAPE("unable to insert the timer in the set");
    }

 inserted:

  /*
   * 5)
   */

  if (machine_call(timer, update, id, delay) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases all the timers.
 *
 * steps:
 *
 * 1) go through the timers.
 *   a) retrieve the timer object.
 *   b) release the timer.
 */

t_status		timer_flush(void)
{
  s_iterator		i;
  t_state		s;

  /*
   * 1)
   */

  set_foreach(SET_OPTION_FORWARD, _timer.timers, &i, s)
    {
      o_timer*		o;

      /*
       * a)
       */

      if (set_object(_timer.timers, i, (void**)&o) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the timer object");

      /*
       * b)
       */

      if (timer_release(o->id) != STATUS_OK)
	CORE_ESCAPE("unable to release the timer");
    }

  CORE_LEAVE();
}

/*
 * this function returns true if the timer object exists.
 */

t_bool			timer_exist(i_timer			id)
{
  if (set_exist(_timer.timers, id) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function retrieves a timer object from the set of timers.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the object from the set.
 */

t_status		timer_get(i_timer			id,
				  o_timer**			object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_get(_timer.timers, id, (void**)object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of timers");

  CORE_LEAVE();
}

/*
 * this function initializes the timer manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the timer manager's structure.
 * 3) initialize the object identifier.
 * 4) reserve the set of timers.
 * 5) call the machine.
 */

t_status		timer_initialize(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the timer manager\n");

  /*
   * 2)
   */

  memset(&_timer, 0x0, sizeof (m_timer));

  /*
   * 3)
   */

  if (id_build(&_timer.id) != STATUS_OK)
    CORE_ESCAPE("unable to build the identifier object");

  /*
   * 4)
   */

  if (set_reserve(ll,
		  SET_OPTION_NONE,
                  sizeof (o_timer),
		  &_timer.timers) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set of timers");

  /*
   * 5)
   */

  if (machine_call(timer, initialize) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the timer manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 * 3) flush the timers.
 * 3) release the set of timers.
 * 5) destroy the identifier object.
 */

t_status		timer_clean(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the timer manager\n");

  /*
   * 2)
   */

  if (machine_call(timer, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  if (timer_flush() != STATUS_OK)
    CORE_ESCAPE("unable to flush the timer");

  /*
   * 4)
   */

  if (set_release(_timer.timers) != STATUS_OK)
    CORE_ESCAPE("unable to release the set of timers");

  /*
   * 5)
   */

  if (id_destroy(&_timer.id) != STATUS_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  CORE_LEAVE();
}
