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

typedef i_thread	pthread_t;

int	pthread_create(pthread_t*,
		       const void*,
		       void *(*)(void*),
		       void*);

void	pthread_yield(void);

#endif
