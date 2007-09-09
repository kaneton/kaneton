/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/libpthread/mutex.c
 *
 * created       matthieu bucchianeri   [thu sep  6 21:55:57 2007]
 * updated       matthieu bucchianeri   [mon sep 10 00:13:06 2007]
 */

#include <pthread.h>
#include <crt.h>
#include <sys/queue.h>
#include <stdlib.h>

int	pthread_mutex_init(pthread_mutex_t*		mutex,
			   const pthread_mutexattr_t*	attr)
{
  mutex->count = 0;
  SIMPLEQ_INIT(&mutex->queue);
  IA32_SPIN_FIELD_INIT(mutex->lock);

  return (0);
}

int	pthread_mutex_lock(pthread_mutex_t*		mutex)
{
  int				res = -1;
  struct __threadqueue_entry*	e;
  i_thread			th;

  IA32_SPIN_LOCK(mutex->lock);

  if (mutex->count)
    {
      if (scheduler_current(&th) != ERROR_NONE)
	goto leave;

      if ((e = malloc(sizeof (*e))) == NULL)
	goto leave;

      e->thread = th;

      SIMPLEQ_INSERT_TAIL(&mutex->queue, e, entry);

      thread_state_unlock(th, SCHEDULER_STATE_STOP, &mutex->lock);

      return (0);
    }
  else
    {
      mutex->count++;
    }

  res = 0;

 leave:
  IA32_SPIN_UNLOCK(mutex->lock);

  return (res);
}

int	pthread_mutex_unlock(pthread_mutex_t*		mutex)
{
  int				res = -1;
  struct __threadqueue_entry*	e;
  i_thread			th;

  IA32_SPIN_LOCK(mutex->lock);

  e = SIMPLEQ_FIRST(&mutex->queue);

  if (e == NULL)
    goto leave;

  mutex->count--;

  th = e->thread;

  SIMPLEQ_REMOVE_HEAD(&mutex->queue, entry);

  free(e);

  thread_state(th, SCHEDULER_STATE_RUN);

  res = 0;

 leave:
  IA32_SPIN_UNLOCK(mutex->lock);

  return (res);
}
