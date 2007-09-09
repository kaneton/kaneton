/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/libpthread/cond.c
 *
 * created       matthieu bucchianeri   [thu sep  6 21:55:24 2007]
 * updated       matthieu bucchianeri   [mon sep 10 00:12:57 2007]
 */

#include <pthread.h>
#include <crt.h>
#include <sys/queue.h>
#include <stdlib.h>

int	pthread_cond_init(pthread_cond_t*		cond,
			  const pthread_condattr_t*	attr)
{
  SIMPLEQ_INIT(&cond->queue);
  IA32_SPIN_FIELD_INIT(cond->lock);

  return (0);
}

int	pthread_cond_wait(pthread_cond_t*		cond,
			  pthread_mutex_t*		mutex)
{
  int				res = -1;
  struct __threadqueue_entry*	e;
  i_thread			th;

  IA32_SPIN_LOCK(cond->lock);

  if (!pthread_mutex_unlock(mutex))
    {
      if (scheduler_current(&th) != ERROR_NONE)
	goto leave;

      if ((e = malloc(sizeof (*e))) == NULL)
	goto leave;

      e->thread = th;

      SIMPLEQ_INSERT_TAIL(&cond->queue, e, entry);

      pthread_mutex_lock(mutex);

      thread_state_unlock(th, SCHEDULER_STATE_STOP, &cond->lock);

      return (0);
    }

 leave:
  IA32_SPIN_UNLOCK(cond->lock);

  return (res);
}

int	pthread_cond_signal(pthread_cond_t*		cond)
{
  struct __threadqueue_entry*	e;
  i_thread			th;

  IA32_SPIN_LOCK(cond->lock);

  e = SIMPLEQ_FIRST(&cond->queue);

  if (e == NULL)
    goto leave;

  th = e->thread;

  SIMPLEQ_REMOVE_HEAD(&cond->queue, entry);

  free(e);

  thread_state(th, SCHEDULER_STATE_RUN);

 leave:
  IA32_SPIN_UNLOCK(cond->lock);

  return (0);
}
