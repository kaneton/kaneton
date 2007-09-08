/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/libpthread/include/pthread.h
 *
 * created       matthieu bucchianeri   [thu sep  6 21:51:43 2007]
 * updated       matthieu bucchianeri   [thu sep  6 21:55:16 2007]
 */

#ifndef PTHREAD_H
#define PTHREAD_H

#include <libkaneton.h>
#include "sys/queue.h"
#include "spinlock.h"

#define PTHREAD_BARRIER_SERIAL_THREAD	42

typedef i_thread	pthread_t;

typedef void		pthread_mutexattr_t;
typedef void		pthread_condattr_t;
typedef void		pthread_barrierattr_t;

SIMPLEQ_HEAD(__threadqueue, __threadqueue_entry);

struct __threadqueue_entry
{
  i_thread				thread;
  SIMPLEQ_ENTRY(__threadqueue_entry)	entry;
};

typedef struct
{
  int			count;
  struct __threadqueue	queue;
  IA32_SPIN_FIELD_DECLARE(lock);
}			pthread_mutex_t;

typedef struct
{
  struct __threadqueue	queue;
  IA32_SPIN_FIELD_DECLARE(lock);
}			pthread_cond_t;

typedef struct
{
  int			count;
  int			initial;
  struct __threadqueue	queue;
  IA32_SPIN_FIELD_DECLARE(lock);
}			pthread_barrier_t;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../thread.c
 *	../barrier.c
 *	../mutex.c
 *	../cond.c
 */

/*
 * ../thread.c
 */

int		pthread_create(pthread_t*	th,
			       const void*	attr,
			       void*		(*start_routine)(void*),
			       void*		arg);

void	pthread_yield(void);


/*
 * ../barrier.c
 */

int	pthread_barrier_init(pthread_barrier_t*			barrier,
			    const pthread_barrierattr_t*	attr,
			    unsigned				count);

int	pthread_barrier_wait(pthread_barrier_t*		barrier);


/*
 * ../mutex.c
 */

int	pthread_mutex_init(pthread_mutex_t*		mutex,
			   const pthread_mutexattr_t*	attr);

int	pthread_mutex_lock(pthread_mutex_t*		mutex);

int	pthread_mutex_unlock(pthread_mutex_t*		mutex);


/*
 * ../cond.c
 */

int	pthread_cond_init(pthread_cond_t*		cond,
			  const pthread_condattr_t*	attr);

int	pthread_cond_wait(pthread_cond_t*		cond,
			  pthread_mutex_t*		mutex);

int	pthread_cond_signal(pthread_cond_t*		cond);


/*
 * eop
 */

#endif
