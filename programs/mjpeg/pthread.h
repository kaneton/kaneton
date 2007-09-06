/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/pthread.h
 *
 * created       matthieu bucchianeri   [sat dec  2 02:27:42 2006]
 * updated       matthieu bucchianeri   [sun dec  3 18:48:26 2006]
 */

#ifndef PTHREAD_H
#define PTHREAD_H

#include <libkaneton.h>
#include "queue.h"
#include "spinlock.h"

#define PTHREAD_BARRIER_SERIAL_THREAD	42

typedef i_thread	pthread_t;

typedef void		pthread_mutexattr_t;
typedef void		pthread_condattr_t;
typedef void		pthread_barrierattr_t;

SIMPLEQ_HEAD(threadqueue, threadqueue_entry);

struct threadqueue_entry
{
  i_thread				thread;
  SIMPLEQ_ENTRY(threadqueue_entry)	entry;
};

typedef struct
{
  int			count;
  struct threadqueue	queue;
  IA32_SPIN_FIELD_DECLARE(lock);
}			pthread_mutex_t;

typedef struct
{
  struct threadqueue	queue;
  IA32_SPIN_FIELD_DECLARE(lock);
}			pthread_cond_t;

typedef struct
{
  int			count;
  int			initial;
  struct threadqueue	queue;
  IA32_SPIN_FIELD_DECLARE(lock);
}			pthread_barrier_t;

int	pthread_create(pthread_t*,
		       const void*,
		       void *(*)(void*),
		       void*);

void	pthread_yield(void);

int	pthread_mutex_init(pthread_mutex_t*,
			   const pthread_mutexattr_t*);

int	pthread_mutex_lock(pthread_mutex_t*);

int	pthread_mutex_unlock(pthread_mutex_t*);

int	pthread_cond_init(pthread_cond_t*,
			  const pthread_condattr_t*);

int	pthread_cond_wait(pthread_cond_t*,
			  pthread_mutex_t*);

int	pthread_cond_signal(pthread_cond_t*);

int	pthread_barrier_init(pthread_barrier_t*,
			     const pthread_barrierattr_t*,
			     unsigned);

int	pthread_barrier_wait(pthread_barrier_t*);


#endif
