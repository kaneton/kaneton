/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/kaneton/timer/timer.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       renaud voltz   [sun feb 12 23:04:54 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the timer manager.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX TIMER
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(timer);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the timer manager variable.
 */

m_timer*                 timer = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given timer.
 *
 * steps:
 *
 * 1) get the timer object from its identifier.
 * 2) display its delay.
 */

t_error			timer_show(t_timerid			timerid)
{

  o_timer*		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (timer_get(timerid, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "  timer %qd: delay = %u\n", o->timerid, o->delay - timer->timeref);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * this function dumps the timers managed by the kernel.
 *
 * steps:
 *
 * 1) get the size of the timer container.
 * 2) display information about every timer.
 */

t_error			timer_dump(void)
{
  t_state		state;
  o_timer*		data;
  t_setsz		size;
  t_iterator		i;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (set_size(timer->container, &size) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  cons_msg('#', "dumping %qd timer(s):\n", size);

  /*
   *  2)
   */

  set_foreach(SET_OPT_FORWARD, timer->container, &i, state)
    {
      if (set_object(timer->container, i, (void**)&data) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);

      if (timer_show(data->timerid) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);
    }

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * notify a task that one of its timers expired.
 */

t_error			timer_notify(t_timerid			timerid)
{
  o_timer*		o;

  TIMER_ENTER(timer);

  if (timer_get(timerid, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /* XXX TIMER
   * send a message to o->taskid */

  printf("timer_notify: timerid = %qd, taskid = %qd\n", timerid, o->taskid);

  if (machdep_call(timer, timer_notify, timerid) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * insert a timer in the timer container.
 *
 * steps:
 *
 * 1) get the number of timers in the container.
 * 2) if the container is empty, just add the new timer and return.
 * 3) seek the right place in the list to insert the new timer.
 * 4) add the new timer in the timer container.
 */

t_error			timer_insert(o_timer*			o)
{
  o_timer*		o_tmp;
  t_setsz		size;
  t_setsz		count = 0;
  t_iterator		iterator;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (set_size(timer->container, &size) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (size == 0)
    {
      if (set_add(timer->container, o) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);

      TIMER_LEAVE(timer, ERROR_NONE);
    }

  /*
   * 3)
   */

  do
    {
      if (!count)
	{
	  if (set_head(timer->container, &iterator) != ERROR_NONE)
	    TIMER_LEAVE(timer, ERROR_UNKNOWN);
	}
      else
	{
	  if (set_next(timer->container, iterator, &iterator) != ERROR_NONE)
	    TIMER_LEAVE(timer, ERROR_UNKNOWN);
	}

      if (set_object(timer->container, iterator, (void**)&o_tmp) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);
    }
  while ((++count != size) && (o_tmp->delay < o->delay));

  /*
   * 4)
   */

  if (o_tmp->delay < o->delay)
    {
      if (set_insert_after(timer->container, iterator, o) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);
    }
  else
    {
      if (set_insert_before(timer->container, iterator, o) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);
    }

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * reserve a timer.
 *
 * steps:
 *
 * 1) create a new timer object and give it an identifier.
 * 2) setup delay and repeat mode.
 * 3) add the new timer to the timer manager.
 */

t_error			timer_reserve(t_tskid			taskid,
				      t_uint32			delay,
				      t_uint32			repeat,
				      t_timerid*		timerid)
{
  o_timer		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_timer));

  if (id_reserve(&timer->id, timerid) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  o.timerid = *timerid;

  o.taskid = taskid;

  /*
   * 2)
   */

  if (repeat)
    o.repeat = delay;

  o.delay = timer->timeref + delay;

  /*
   * 3)
   */

  if (timer_insert(&o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machdep_call(timer, timer_reserve, taskid, delay, repeat, timerid)
      != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * this function releases a timer.
 *
 * steps:
 *
 * 1) call the machine dependant code.
 * 2) destroy the identifier object.
 * 3) remove the timer object from the timer container.
 */

t_error			timer_release(t_timerid			timerid)
{
  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (machdep_call(timer, timer_release, timerid) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (id_release(&timer->id, timerid) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (set_remove(timer->container, timerid) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * Get the remaining time before expiration date.
 *
 * steps:
 *
 * 1) call the machdep code.
 * 2) get the timer object and return its delay.
 */

t_error			timer_check(t_timerid			timerid,
				   t_uint32*			delay)
{
  o_timer*		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (machdep_call(timer, timer_check, timerid, delay) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (timer_get(timerid, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  *delay = o->delay;

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * modify a timer attributes.
 *
 * steps:
 *
 * 1) check if the timer exists and get it.
 * 2) clone the timer and update its delay and repeat mode.
 * 3) reorganize the timer position within the container.
 * 4) call the machdep code.
 */

t_error			timer_modify(t_timerid			timerid,
				    t_uint32			delay,
				    t_uint32			repeat)
{
  o_timer		t;
  o_timer*		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (timer_get(timerid, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  memcpy(&t, o, sizeof(o_timer));

  if (repeat)
    t.repeat = delay;

  t.delay = timer->timeref + delay;

  /*
   * 3)
   */

  if (timer_release(timerid) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  if (timer_insert(&t) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machdep_call(timer, timer_modify, timerid, delay, repeat) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * this function get an timer object from the timer container.
 */

t_error			timer_get(t_timerid			timerid,
				  o_timer**			o)
{
  TIMER_ENTER(timer);

  if (set_get(timer->container, timerid, (void**)o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * initialize the timer manager.
 *
 * steps:
 *
 * 1) allocate and initialize the timer manager.
 * 2) initialize the object identifier.
 * 3) reserve the timer container.
 * 4) try to reserve a statistic object.
 * 5) call the machine dependent code.
 */

t_error			timer_init(void)
{

  /*
   * 1)
   */

  if ((timer = malloc(sizeof(m_timer))) == NULL)
    {
      cons_msg('!', "timer: cannot allocate memory for the timer manager "
               "structure\n");

      return ERROR_UNKNOWN;
    }

  memset(timer, 0x0, sizeof(m_timer));

  /*
   * 2)
   */

  if (id_build(&timer->id) != ERROR_NONE)
    {
      cons_msg('!', "timer: unable to initialize the identifier object\n");

      return ERROR_UNKNOWN;
    }

  timer->timeref = 0;

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC,
                  sizeof(o_timer), &timer->container) != ERROR_NONE)
    {
      cons_msg('!', "timer: unable to reserve the timer container\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 4)
   */

  STATS_RESERVE("timer", &timer->stats);

  /*
   * 5)
   */

  if (machdep_call(timer, timer_init) != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * destroy the timer manager.
 *
 * steps:
 *
 * 1) call the machine dependent code.
 * 2) release the statistics object.
 * 3) release the timer container.
 * 4) destroy the identifier object.
 * 5) free the timer manager's structure memory.
 */

t_error			timer_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(timer, timer_clean) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  STATS_RELEASE(timer->stats);

  /*
   * 3)
   */

  if (set_release(timer->container) != ERROR_NONE)
    {
      cons_msg('!', "timer: unable to release the timer container\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 4)
   */

  if (id_destroy(&timer->id) != ERROR_NONE)
    {
      cons_msg('!', "timer: unable to destroy the identifier object\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 5)
   */

  free(timer);

  return ERROR_NONE;
}

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

t_error			timer_update(void)
{
  o_timer*		o;
  t_iterator		iterator;

  TIMER_ENTER(timer);

  while (1)
    {
      /*
       * 1)
       */

      if (set_head(timer->container, &iterator) != ERROR_NONE)
        TIMER_LEAVE(timer, ERROR_UNKNOWN);


      if (set_object(timer->container, iterator, (void**)&o) != ERROR_NONE)
        TIMER_LEAVE(timer, ERROR_UNKNOWN);

      /*
       * 2)
       */

      if (timer->timeref < o->delay)
        break;

      /*
       * 3)
       */

      if (timer_notify(o->timerid) != ERROR_NONE)
        TIMER_LEAVE(timer, ERROR_UNKNOWN);

      /*
       * 4)
       */

      if (o->repeat)
        {
          if (timer_modify(o->timerid, o->repeat, 1) != ERROR_NONE)
            TIMER_LEAVE(timer, ERROR_UNKNOWN);
        }
      else
        {
          if (timer_release(o->timerid) != ERROR_NONE)
            TIMER_LEAVE(timer, ERROR_UNKNOWN);
        }
    }

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * manage the timer interrupt.
 *
 * steps:
 *
 * 1) update the timer reference.
 * 2) check if a timer has expired.
 * 3) call the scheduler.
 */

void			timer_handler(t_uint32		id)
{
  /*
   * 1)
   */

  timer->timeref++;

  /*
   * 2)
   */

  timer_update();

  /*
   * 3)
   */

  /* XXX should call the scheduler */
}

/*
 * XXX TIMERS: remove me !
 * just for testing
 */

t_error			timer_test(void)
{
  t_timerid		timerid;

  if (timer_reserve(23, 500, 1, &timerid) != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (timer_reserve(42, 100, 1, &timerid) != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (timer_dump() != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*                                                                  [cut] k3 */
