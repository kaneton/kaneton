/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/libpthread/barrier.c
 *
 * created       matthieu bucchianeri   [thu sep  6 21:54:23 2007]
 * updated       matthieu bucchianeri   [mon sep 10 00:12:50 2007]
 */

#include <pthread.h>
#include <crt.h>
#include <sys/queue.h>
#include <stdlib.h>

int	pthread_barrier_init(pthread_barrier_t*			barrier,
			    const pthread_barrierattr_t*	attr,
			    unsigned				count)
{
  barrier->count = barrier->initial = count;
  SIMPLEQ_INIT(&barrier->queue);
  IA32_SPIN_FIELD_INIT(barrier->lock);

  return (0);
}

int	pthread_barrier_wait(pthread_barrier_t*		barrier)
{
  int				res = -1;
  struct __threadqueue_entry*	e;
  i_thread			th;

  IA32_SPIN_LOCK(barrier->lock);

  barrier->count--;

  if (barrier->count == 0)
    {
      while ((e = SIMPLEQ_FIRST(&barrier->queue)) != NULL)
	{
	  th = e->thread;

	  SIMPLEQ_REMOVE_HEAD(&barrier->queue, entry);

	  free(e);

	  thread_state(th, SCHEDULER_STATE_RUN);
	}

      barrier->count = barrier->initial;

      res = PTHREAD_BARRIER_SERIAL_THREAD;

      goto leave;
    }
  else
    {
      if (scheduler_current(&th) != ERROR_NONE)
	goto leave;

      if ((e = malloc(sizeof (*e))) == NULL)
	goto leave;

      e->thread = th;

      SIMPLEQ_INSERT_TAIL(&barrier->queue, e, entry);

      thread_state_unlock(th, SCHEDULER_STATE_STOP, &barrier->lock);

      return (0);
    }

  res = 0;
 leave:
  IA32_SPIN_UNLOCK(barrier->lock);

  return (res);
}
