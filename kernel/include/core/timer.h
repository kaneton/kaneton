/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/kaneton/timer.h
 *
 * created       renaud voltz   [sun feb 12 22:26:04 2006]
 * updated       julien quintard   [fri mar 10 04:02:19 2006]
 */

#ifndef KANETON_TIMER_H
#define KANETON_TIMER_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/id.h>
#include <kaneton/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * timer object
 */

typedef struct
{
  t_timerid			timerid;

  t_uint32			delay;

  t_uint32			repeat;

  t_tskid			taskid;

  machdep_data(o_timer);
}				o_timer;


/*
 * timer manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;

  t_uint32			timeref;

  t_setid			container;

  machdep_data(m_timer);
}				m_timer;

/*
 * the timer architecture dependent interface
 */

typedef struct
{
  t_error			(*timer_show)(t_timerid);
  t_error			(*timer_notify)(t_timerid);
  t_error			(*timer_check)(t_timerid,
					       t_uint32*);
  t_error			(*timer_modify)(t_timerid,
						t_uint32,
						t_uint32);
  t_error			(*timer_reserve)(t_tskid,
						 t_uint32,
						 t_uint32,
						 t_timerid*);
  t_error			(*timer_release)(t_timerid);
  t_error			(*timer_init)(void);
  t_error			(*timer_clean)(void);
}				i_timer;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define TIMER_CHECK(_timer_)						\
  {									\
    if ((_timer_) == NULL)						\
      return ERROR_UNKNOWN;						\
  }

/*
 * enter
 */

#define TIMER_ENTER(_timer_)						\
  {									\
    TIMER_CHECK((_timer_));						\
									\
    STATS_BEGIN((_timer_)->stats);					\
  }

/*
 * leave
 */

#define TIMER_LEAVE(_timer_, _error_)					\
  {									\
    STATS_END((_timer_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/timer/timer.c
 */

/*
 * ../../kaneton/time/timer.c
 */

t_error			timer_show(t_timerid			timerid);

t_error			timer_dump(void);

t_error			timer_notify(t_timerid			timerid);

t_error			timer_insert(o_timer*			o);

t_error			timer_reserve(t_tskid			taskid,
				      t_uint32			delay,
				      t_uint32			repeat,
				      t_timerid*		timerid);

t_error			timer_release(t_timerid			timerid);

t_error			timer_check(t_timerid			timerid,
				   t_uint32*			delay);

t_error			timer_modify(t_timerid			timerid,
				    t_uint32			delay,
				    t_uint32			repeat);

t_error			timer_get(t_timerid			timerid,
				  o_timer**			o);

t_error			timer_init(void);

t_error			timer_clean(void);

t_error			timer_update(void);

void			timer_handler(t_uint32		id);

t_error			timer_test(void);


/*
 * eop
 */

#endif
