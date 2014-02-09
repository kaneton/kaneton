/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/timer.h
 *
 * created       julien quintard   [wed jun  6 15:42:26 2007]
 * updated       julien quintard   [wed mar  2 20:27:39 2011]
 */

#ifndef CORE_TIMER_H
#define CORE_TIMER_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the different types of timers: either a function is called within the
 * kernel or a message is sent to a task.
 */

#define TIMER_TYPE_FUNCTION	0
#define TIMER_TYPE_MESSAGE	1

/*
 * the timer options.
 *
 * the 'repeat' option makes the timer go repeatdly.
 */

#define TIMER_OPTION_NONE	0
#define TIMER_OPTION_REPEAT	1

/*
 * the number of millisecond between each tick: the timers are checked
 * after this delay. in other words this is the base unit for times but
 * also for the scheduler.
 */

#define TIMER_DELAY		25

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function casts a function pointer into a proper timer
 * handler.
 */

#define TIMER_ROUTINE(_routine_)					\
  ((u_timer_handler)(t_timer_routine)(_routine_))

/*
 * this macro-function casts a task identifier into a proper timer
 * handler.
 */

#define TIMER_TASK(_task_)						\
  ((u_timer_handler)(i_task)(_task_))

/*
 * this macro-function casts an arbitrary data into a valid timer data.
 */

#define TIMER_DATA(_data_)						\
  (t_data)((_data_))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this type represents a routine-specific timer handler.
 */

typedef t_status		(*t_timer_routine)(i_timer,
						   t_data);

/*
 * this type is the generic timer handler.
 */

typedef union
{
  t_timer_routine		routine;
  i_task			task;
}				u_timer_handler;

/*
 * the timer object which is identifier by 'id'.
 *
 * the 'delay' indicates after how many milliseconds the associated
 * 'handler' of type 'type' must be triggered. note that some arbitrary
 * data 'data' are passed to the handler. these data are provided by
 * the user when the timer is reserved.
 */

typedef struct
{
  i_timer			id;

  t_type			type;

  t_delay			delay;

  t_options			options;
  t_delay			repeat;

  u_timer_handler		handler;
  t_data			data;

  machine_data(o_timer);
}				o_timer;

/*
 * the message object for message-based events.
 */

typedef struct
{
  i_timer			id;

  t_data			data;
}				o_timer_message;

/*
 * the timer manager.
 *
 * the 'reference' is the number of milliseconds elapsed since the
 * timer manager initialisation. each tick triggers the timer manager's
 * handler which increases the reference. then, for every expired timer,
 * its associated handler is triggered.
 */

typedef struct
{
  o_id				id;

  i_set				timers;

  machine_data(m_timer);
}				m_timer;

/*
 * the timer dispatcher.
 */

typedef struct
{
  t_status		(*timer_show)(i_timer,
					      mt_margin);
  t_status		(*timer_dump)(void);
  t_status		(*timer_notify)(i_timer);
  t_status		(*timer_reserve)(t_type,
						 u_timer_handler,
						 t_data,
						 t_delay,
						 t_options,
						 i_timer*);
  t_status		(*timer_release)(i_timer);
  t_status		(*timer_update)(i_timer,
						t_delay);
  t_status		(*timer_initialize)(void);
  t_status		(*timer_clean)(void);
}				d_timer;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/timer/timer.c
 */

/*
 * ../../core/timer/timer.c
 */

void			timer_handler(i_event			event,
				      t_data			data);

t_status		timer_check(void);

t_status		timer_show(i_timer			id,
				   mt_margin			margin);

t_status		timer_dump(void);

t_status		timer_notify(i_timer			id);

t_status		timer_reserve(t_type			type,
				      u_timer_handler		handler,
				      t_data			data,
				      t_delay			delay,
				      t_options			options,
				      i_timer*			id);

t_status		timer_release(i_timer			id);

t_status		timer_update(i_timer			id,
				     t_delay			delay);

t_status		timer_flush(void);

t_bool			timer_exist(i_timer			id);

t_status		timer_get(i_timer			id,
				  o_timer**			object);

t_status		timer_initialize(void);

t_status		timer_clean(void);


/*
 * eop
 */

#endif
