/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/time/timer.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       matthieu bucchianeri   [tue feb  6 23:16:26 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the timer manager.
 * a timer is parameterised around three properties: the task it belongs to,
 * its delay and its repeat mode. When its delay (in 1/100 of sec) has expired,
 * a message is sent to the task taskid before behaving as follow:
 * a) if the timer options are set to TIMER_OPTION_ENABLE, the timer is
 *    automaticaly reset to its original delay.
 * b) if not, the timer is released.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(timer);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the timer manager variable.
 */

m_timer*		_timer = NULL;

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * check timer expiration.
 *
 * steps:
 *
 * 1) get the timer with the closest expiration date.
 * 2) return if this timer has not expired yet.
 * 3) otherwise, notify the task its timer has expired.
 * 4) release the timer, or re-initialize it if repeat mode is enabled.
 */

t_error			timer_check(void)
{
  o_timer*		o;
  t_iterator		i;

  while (set_head(_timer->timers, &i) == ERROR_TRUE)
    {

      /*
       * 1)
       */

      if (set_object(_timer->timers, i, (void**)&o) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the timer");

      /*
       * 2)
       */

      if (_timer->reference < o->delay)
        break;

      /*
       * 3)
       */

      if (o->type == EVENT_MESSAGE)
	{
	  if (timer_notify(o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to notify the task of the timer's expiration");
	}
      else
	{
	  o->handler.function(o->id, o->data);
	}

      /*
       * 4)
       */

      if (o->options & TIMER_OPTION_REPEAT)
        {
	  if (timer_modify(o->id, o->repeat, o->options) != ERROR_OK)
	    CORE_ESCAPE("unable to modify the timer");
        }
      else
        {
          if (timer_release(o->id) != ERROR_OK)
	    CORE_ESCAPE("unable to release the timer");
        }
    }

  CORE_LEAVE();
}

/*
 * this is the handler triggers whenever a timer event occurs. this is
 * used to keep track of time and trigger the registered timers.
 *
 * steps:
 *
 * 1) update the elapsed time.
 * 2) update the clock.
 * 3) trigger the expired timers.
 */

void			timer_handler(t_id			id)
{
  /*
   * 1)
   */

  _timer->reference += TIMER_DELAY;

  /*
   * 2)
   */

  assert(clock_update(TIMER_DELAY));

  /*
   * 3)
   */

  assert(timer_check());
}

/*
 * this function shows a given timer.
 *
 * steps:
 *
 * 1) get the timer object from its identifier.
 * 2) display its delay.
 *
 */

t_error			timer_show(i_timer			id)
{
  o_timer*		o;

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "  timer %qd: delay = %qu\n",
	      o->id, o->delay - _timer->reference);

  CORE_LEAVE();
}

/*
 * this function dumps the timers managed by the kernel.
 *
 * steps:
 *
 * 1) get the size of the timer set.
 * 2) display information about every timer.
 *
 */

t_error			timer_dump(void)
{
  t_state		state;
  o_timer*		data;
  t_setsz		size;
  t_iterator		i;

  /*
   * 1)
   */

  if (set_size(_timer->timers, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of timers");

  module_call(console, console_message,
	      '#', "dumping %qd timer(s):\n",
	      size);

  /*
   *  2)
   */

  set_foreach(SET_OPTION_FORWARD, _timer->timers, &i, state)
    {
      if (set_object(_timer->timers, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the timer");

      if (timer_show(data->id) != ERROR_OK)
	CORE_ESCAPE("unable to show the timer");
    }

  CORE_LEAVE();
}

/*
 * notify a task that one of its timers expired.
 *
 * steps:
 *
 * 1) get the timer object from the set.
 * 2) notify the task for its timer expiration.
 * 3) call the machine-dependent code.
 *
 */

t_error			timer_notify(i_timer			id)
{
  o_timer*		o;
  o_timer_message	msg;
  i_node		node;

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 2)
   */

  msg.id = id;
  msg.data = o->data;
  node.machine = _kernel->machine;
  node.task = o->handler.task;

  if (message_send(_kernel->task, node, MESSAGE_TYPE_TIMER, (t_vaddr)&msg,
		   sizeof (o_timer_message)) != ERROR_OK)
    CORE_ESCAPE("unable to send a message to the task");

  /*
   * 3)
   */

  if (machine_call(timer, timer_notify, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * insert a timer in the timer set.
 *
 * steps:
 *
 * 1) insert the timer before the first which has a greater expiration date.
 * 2) if there is no timer or every timer expire sooner, insert it at the end.
 */

t_error			timer_insert(o_timer*			o)
{
  t_state		state;
  o_timer*		o_tmp;
  t_iterator		i;

  assert(o != NULL);

  /*
   * 1)
   */

  set_foreach(SET_OPTION_FORWARD, _timer->timers, &i, state)
    {
      if (set_object(_timer->timers, i, (void**)&o_tmp) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the timer");

      if (o_tmp->delay >= o->delay)
	{
	  if (set_before(_timer->timers, i, o) != ERROR_OK)
	    CORE_ESCAPE("unable to insert the timer in the set");

	  CORE_LEAVE();
	}
    }

  /*
   * 2)
   */

  if (set_append(_timer->timers, o) != ERROR_OK)
    CORE_ESCAPE("unable to append the timer object at the end of the set");

  CORE_LEAVE();
}

/*
 * reserve a timer.
 *
 * steps:
 *
 * 1) create a new timer object and give it an identifier.
 * 2) setup delay and repeat mode.
 * 3) add the new timer to the timer manager.
 * 4) call the machine-dependent code.
 *
 */

t_error			timer_reserve(t_type			type,
				      u_timer_handler		handler,
				      t_vaddr			data,
				      t_uint32			delay,
				      t_options			options,
				      i_timer*			id)
{
  o_timer		o;

  assert((type == EVENT_FUNCTION) || (type == EVENT_MESSAGE));
  assert(id != NULL);

  /*
   * 1)
   */

  if (id_reserve(&_timer->id, id) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a timer identifier");

  memset(&o, 0x0, sizeof(o_timer));

  o.id = *id;
  o.type = type;
  o.handler = handler;
  o.data = data;
  o.options = options;

  /*
   * 2)
   */

  if (o.options & TIMER_OPTION_REPEAT)
    o.repeat = delay;

  o.delay = _timer->reference + delay;

  /*
   * 3)
   */

  if (timer_insert(&o) != ERROR_OK)
    CORE_ESCAPE("unable to insert the object in the set of timers");

  /*
   * 4)
   */

  if (machine_call(timer,
		   timer_reserve,
		   type,
		   handler,
		   data,
		   delay,
		   options,
		   id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases a timer.
 *
 * steps:
 *
 * 1) check that the timer exists.
 * 2) call the machine dependant code.
 * 3) destroy the identifier object.
 * 4) remove the timer object from the timer set.
 *
 */

t_error			timer_release(i_timer			id)
{
  o_timer*		o;

  /*
   * 1)
   */

  /*
   * 2)
   */

  if (machine_call(timer, timer_release, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  if (timer_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 3)
   */

  if (id_release(&_timer->id, id) != ERROR_OK)
    CORE_ESCAPE("unable to release the timer identifier");

  /*
   * 4)
   */

  if (set_remove(_timer->timers, id) != ERROR_OK)
    CORE_ESCAPE("unable to remove the object from the set of timers");

  CORE_LEAVE();
}

/*
 * set the timer delay.
 *
 * steps:
 *
 * 1) get the timer object.
 * 2) set the delay.
 # 3) call the machine-dependent code.
 *
 */

t_error			timer_delay(i_timer			id,
				    t_uint32			delay)
{
  o_timer*		o;

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 2)
   */

  o->delay = delay;

  /*
   * 3)
   */

  if (machine_call(timer, timer_delay, id, delay) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * set the timer repeat mode.
 *
 * steps:
 *
 * 1) get the timer object.
 * 2) set the repeat mode.
 * 3) call the machine-dependent code.
 *
 */

t_error			timer_repeat(i_timer			id,
				     t_uint64			repeat)
{
  o_timer*		o;

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 2)
   */

  o->options |= TIMER_OPTION_REPEAT;
  o->repeat = repeat;

  /*
   * 3)
   */

  if (machine_call(timer, timer_repeat, id, repeat) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * modify a timer attributes.
 *
 * steps:
 *
 * 1) check if the timer exists and get it.
 * 2) call the machine code.
 * 3) clone the timer and update its delay and repeat mode.
 * 4) reorganize the timer position within the set.
 *
 */

t_error			timer_modify(i_timer			id,
				     t_uint64			delay,
				     t_options			options)
{
  o_timer		o;
  o_timer*		old;

  /*
   * 1)
   */

  if (timer_get(id, &old) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the timer object");

  /*
   * 2)
   */

  if (machine_call(timer, timer_modify, id, delay, options) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  memcpy(&o, old, sizeof(o_timer));

  o.options = options;

  if (o.options & TIMER_OPTION_REPEAT)
    o.repeat = delay;

  o.delay = _timer->reference + delay;

  /*
   * 4)
   */

  if (timer_release(id) != ERROR_OK)
    CORE_ESCAPE("unable to release the timer");

  if (timer_insert(&o) != ERROR_OK)
    CORE_ESCAPE("unable to insert the object in the set of timers");

  CORE_LEAVE();
}

/*
 * this function returns true if the timer object exists.
 *
 */

t_error			timer_exist(i_timer			id)
{
  if (set_exist(_timer->timers, id) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function finds a timer object in the timer set.
 *
 */

t_error			timer_get(i_timer			id,
				  o_timer**			o)
{
  assert(o != NULL);

  if (set_get(_timer->timers, id, (void**)o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of timers");

  CORE_LEAVE();
}

/*
 * initialize the timer manager.
 *
 * steps:
 *
 * 1) allocate and initialize the timer manager.
 * 2) initialize the object identifier.
 * 3) reserve the timer set.
 * 4) call the machine dependent code.
 *
 */

t_error			timer_initialize(void)
{
  /*
   * 1)
   */

  if ((_timer = malloc(sizeof(m_timer))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the timer manger's structure");

  memset(_timer, 0x0, sizeof(m_timer));

  /*
   * 2)
   */

  if (id_build(&_timer->id) != ERROR_OK)
    CORE_ESCAPE("unable to build the identifier object");

  _timer->reference = 100;

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPTION_ALLOC,
                  sizeof(o_timer), &_timer->timers) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of timers");

  /*
   * 4)
   */

  if (machine_call(timer, timer_initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * destroy the timer manager.
 *
 * steps:
 *
 * 1) call the machine dependent code.
 * 2) release the timer set.
 * 3) destroy the identifier object.
 * 4) free the timer manager's structure memory.
 *
 */

t_error			timer_clean(void)
{
  /*
   * 1)
   */

  if (machine_call(timer, timer_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (set_release(_timer->timers) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of timers");

  /*
   * 3)
   */

  if (id_destroy(&_timer->id) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  /*
   * 4)
   */

  free(_timer);

  CORE_LEAVE();
}
