/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/pthread.c
 *
 * created       matthieu bucchianeri   [sat dec  2 02:27:13 2006]
 * updated       matthieu bucchianeri   [wed dec 13 15:15:50 2006]
 */

#include <pthread.h>
#include <crt.h>
#include <libc.h>

int		pthread_create(pthread_t*	th,
			       const void*	attr,
			       void*		(*start_routine)(void*),
			       void*		arg)
{
  t_thread_context	ctx;
  t_stack	stack;
  t_vaddr	stk;

  if (thread_reserve(_crt_get_task_id(), THREAD_PRIOR, th) != ERROR_NONE)
    return (-1);

  stack.base = 0;
  stack.size = 4 * PAGESZ;
  if (thread_stack(*th, stack) != ERROR_NONE)
    return (-1);

  if (thread_attribute_stack(*th, &stack.base) != ERROR_NONE)
    return (-1);

  stk = stack.base + stack.size - 16;

  ctx.sp = stk - 8;
  ctx.pc = (t_uint32)start_routine;
  if (thread_load(*th, ctx) != ERROR_NONE)
    return (-1);

  if (thread_args(*th, &arg, sizeof (void*)) != ERROR_NONE)
    return (-1);

  return 0;
}

void	pthread_yield(void)
{
  scheduler_yield(0);
}

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
  struct threadqueue_entry*	e;
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
  struct threadqueue_entry*	e;
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
  struct threadqueue_entry*	e;
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
  struct threadqueue_entry*	e;
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
