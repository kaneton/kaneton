#include <timer.h>
#include <crt.h>

struct timer_s timer_ms;

error_t	timer_add_event(struct timer_s		*timer,
			struct timer_event_s	*event)
{
  if (timer_reserve(TIMER_MESSAGE, TIMER_TASK(_crt_get_task_id()),
		    (t_vaddr)event, event->delay, 0, &event->id) != ERROR_NONE)
    return (-1);

  return (0);
}

error_t	timer_cancel_event(struct timer_event_s	*event,
			   bool_t		callback)
{
  if (timer_release(event->id) != ERROR_NONE)
    return (-1);

  if (callback)
    event->callback(event, event->pv);

  return (0);
}

timer_delay_t	timer_get_tick(struct timer_s		*timer)
{
  /* XXX */

  return 0;
}

void timer_init(void)
{
  /* XXX */
}
