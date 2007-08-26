/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/mwmr.c
 *
 * created       matthieu bucchianeri   [sat dec  2 02:00:48 2006]
 * updated       matthieu bucchianeri   [wed dec 13 03:12:07 2006]
 */

#include <libc.h>
#include <libkaneton.h>
#include <crt.h>
#include "pthread.h"

#include "dsx.h"

#define WORD_SIZE 4

void	dsx_mwmr_alloc(dsx_mwmr_t	fifo,
		       unsigned int	w,
		       unsigned int	depth,
		       const char*	name)
{

  t_vaddr addr;

  if (map_reserve(_crt_get_as_id(), MAP_OPT_NONE | MAP_OPT_USER, 4096, PERM_READ | PERM_WRITE ,&addr) != ERROR_NONE)
    {
      printf("cannot map_reserve\n");

      while (1)
	;
    }

  pthread_mutex_init(&fifo->lock, NULL);
  pthread_cond_init(&fifo->nempty, NULL);
  pthread_cond_init(&fifo->nfull, NULL);
  fifo->offset = 0;
  fifo->usage = 0;
  fifo->name = name;
  fifo->begin = fifo->rptr = fifo->wptr = (void*)addr;
  fifo->end = fifo->begin + w * depth;
  fifo->width = w;
  fifo->depth = depth;
}

void	_dsx_mwmr_reset(dsx_mwmr_t	fifo,
			const char*	file,
			const int	line)
{
  pthread_mutex_lock(&fifo->lock);

  fifo->usage = 0;
  fifo->rptr = fifo->wptr = fifo->begin;

  pthread_cond_signal(&fifo->nempty);
  pthread_mutex_unlock(&fifo->lock);
}

void	_dsx_mwmr_read(dsx_mwmr_t	fifo,
		       unsigned char*	mem,
		       unsigned int	len,
		       const char*	file,
		       const int	line)
{
  unsigned int	get = 0;
  unsigned int *ptr = (unsigned int *)mem;

  pthread_mutex_lock(&fifo->lock);
  len /= fifo->width;

  while (get < len)
    {
      while (!fifo->usage)
	pthread_cond_wait(&fifo->nempty, &fifo->lock);
      memcpy(ptr, fifo->rptr, fifo->width * WORD_SIZE);
      fifo->rptr += fifo->width;
      fifo->offset += fifo->width;
      if (fifo->rptr == fifo->end)
	fifo->rptr = fifo->begin;
      fifo->usage -= 1;
      pthread_cond_signal(&fifo->nfull);
      ++get;
      ptr += fifo->width;

    }
  pthread_mutex_unlock(&fifo->lock);
  pthread_cond_signal(&fifo->nfull);
}

void	_dsx_mwmr_write(dsx_mwmr_t	fifo,
			unsigned char*	mem,
			unsigned int	len,
			const char*	file,
			const int	line)
{
  unsigned int	put = 0;
  unsigned int *ptr = (unsigned int *)mem;

  pthread_mutex_lock(&fifo->lock);
  len /= fifo->width;

  while (put < len)
    {
      while ( fifo->usage == fifo->depth )
	pthread_cond_wait(&fifo->nfull, &fifo->lock);
      memcpy(fifo->wptr, ptr, fifo->width * WORD_SIZE);
      fifo->wptr += fifo->width;
      if (fifo->wptr == fifo->end)
	fifo->wptr = fifo->begin;
      fifo->usage += 1;
      pthread_cond_signal(&fifo->nempty);
      ++put;
      ptr += fifo->width;
    }
    pthread_mutex_unlock(&fifo->lock);
    pthread_cond_signal(&fifo->nempty);

    pthread_yield();
}
