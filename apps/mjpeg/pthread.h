/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/mjpeg/pthread.h
 *
 * created       matthieu bucchianeri   [sat dec  2 02:27:42 2006]
 * updated       matthieu bucchianeri   [sat dec  2 02:31:05 2006]
 */

#ifndef PTHREAD_H
#define PTHREAD_H

#include <kaneton.h>

typedef t_id	pthread_t;

int	pthread_create(pthread_t*,
		       const void*,
		       void *(*)(void*),
		       void*);

void	pthread_yield(void);

#endif
