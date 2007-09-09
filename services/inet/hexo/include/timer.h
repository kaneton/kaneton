#ifndef TIMER_H_
#define TIMER_H_

#include <stdlib.h>
#include <hexo/types.h>
#include <hexo/error.h>
#undef ERROR_UNKNOWN
#include <libkaneton.h>

struct timer_event_s;

/*
 * typedef for timer delays.
 */

typedef uint32_t	timer_delay_t;

/*
 * callback functions prototype.
 */

#define TIMER_CALLBACK(f)	void (f) (struct timer_event_s	*timer,	\
					  void			*pv)

typedef TIMER_CALLBACK(timer_event_callback_t);

/*
 * this structure declares a timer with its delay and callback.
 */

struct				timer_event_s
{
  timer_event_callback_t	*callback;
  void				*pv;
  timer_delay_t			start;
  timer_delay_t			delay;

  i_timer			id;
};

struct				timer_s
{

};

error_t	timer_add_event(struct timer_s		*timer,
			struct timer_event_s	*event);
error_t	timer_cancel_event(struct timer_event_s	*event,
			   bool_t		callback);
timer_delay_t	timer_get_tick(struct timer_s		*timer);

void timer_init(void);

extern struct timer_s timer_ms;

#endif
