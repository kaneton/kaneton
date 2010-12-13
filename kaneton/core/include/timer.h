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
 * updated       julien quintard   [fri dec 10 15:26:53 2010]
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
 * timer handling way
 */

#define TIMER_TYPE_FUNCTION	0
#define TIMER_TYPE_MESSAGE	1

/*
 * timer options.
 */

#define TIMER_OPTION_NONE	0
#define TIMER_OPTION_REPEAT	1

/*
 * number of millisecond between each tick.
 */

#define TIMER_DELAY		25

/*
 * ---------- macro functions -------------------------------------------------
 */

#define TIMER_ROUTINE(_routine_)					\
  ((u_timer_handler)(t_timer_routine)(_routine_))

#define TIMER_TASK(_task_)						\
  ((u_timer_handler)(i_task)(_task_))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * generic timer handler type
 */

typedef t_error			(*t_timer_routine)(i_timer,
						   t_vaddr);

/*
 * timer handler type
 */

typedef union
{
  t_timer_routine		routine;
  i_task			task;
}				u_timer_handler;

/*
 * timer object
 */

typedef struct
{
  i_timer			id;

  t_uint64			delay;

  t_options			options;
  t_uint64			repeat;

  t_type			type;

  u_timer_handler		handler;
  t_vaddr			data;

  machine_data(o_timer);
}				o_timer;

/*
 * message object for event
 */

typedef struct
{
  i_timer			id;

  t_vaddr			data;
}				o_timer_message;

/*
 * timer manager
 */

typedef struct
{
  o_id				id;

  t_uint64			reference;

  i_set				timers;

  machine_data(m_timer);
}				m_timer;

/*
 * the timer architecture dependent interface
 */

typedef struct
{
  t_error			(*timer_show)(i_timer);
  t_error			(*timer_delay)(i_timer,
					       t_uint32);
  t_error			(*timer_repeat)(i_timer,
						t_uint32);
  t_error			(*timer_modify)(i_timer,
						t_uint32,
						t_uint32);
  t_error			(*timer_notify)(i_timer);
  t_error			(*timer_reserve)(t_type,
						 u_timer_handler,
						 t_vaddr,
						 t_uint32,
						 t_uint32,
						 i_timer*);
  t_error			(*timer_release)(i_timer);
  t_error			(*timer_initialize)(void);
  t_error			(*timer_clean)(void);
}				d_timer;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/timer/timer.c
 */

/*
 * ../../core/timer/timer.c
 */

t_error			timer_check(void);

void			timer_handler(t_id			id);

t_error			timer_show(i_timer			id);

t_error			timer_dump(void);

t_error			timer_notify(i_timer			id);

t_error			timer_insert(o_timer*			o);

t_error			timer_reserve(t_type			type,
				      u_timer_handler		handler,
				      t_vaddr			data,
				      t_uint32			delay,
				      t_options			options,
				      i_timer*			id);

t_error			timer_release(i_timer			id);

t_error			timer_delay(i_timer			id,
				    t_uint32			delay);

t_error			timer_repeat(i_timer			id,
				     t_uint64			repeat);

t_error			timer_modify(i_timer			id,
				     t_uint64			delay,
				     t_options			options);

t_error			timer_exist(i_timer			id);

t_error			timer_get(i_timer			id,
				  o_timer**			o);

t_error			timer_initialize(void);

t_error			timer_clean(void);


/*
 * eop
 */

#endif
