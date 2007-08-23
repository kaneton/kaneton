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
#include "spinlock.h"

#include "dsx.h"

#define WORD_SIZE 4

#if 1
#define BLOCK(_chiche_)							\
  {									\
    if ((_chiche_) != -1)						\
      thread_state((_chiche_), SCHEDULER_STATE_STOP);			\
  }


#define UNBLOCK(_chiche_)						\
  {									\
    if ((_chiche_) != -1)						\
      thread_state((_chiche_), SCHEDULER_STATE_RUN);			\
  }

#define YIELD()		scheduler_yield(0);
#endif

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

  printf("mapped %p\n", addr);

  fifo->offset = 0;
  fifo->usage = 0;
  fifo->name = name;
  fifo->begin = fifo->rptr = fifo->wptr = (void*)addr;
  fifo->end = fifo->begin + w * depth;
  fifo->width = w;
  fifo->depth = depth;
  IA32_SPIN_FIELD_INIT(fifo->lock);
  fifo->reader = -1;
  fifo->writer = -1;
}

void	_dsx_mwmr_reset(dsx_mwmr_t	fifo,
			const char*	file,
			const int	line)
{
  IA32_SPIN_LOCK(fifo->lock);

  fifo->usage = 0;
  fifo->rptr = fifo->wptr = fifo->begin;
  UNBLOCK(fifo->writer);

  IA32_SPIN_UNLOCK(fifo->lock);
}

void	_dsx_mwmr_read(dsx_mwmr_t	fifo,
		       unsigned char*	mem,
		       unsigned int	len,
		       const char*	file,
		       const int	line)
{
  unsigned int	get = 0;
  unsigned int *ptr = (unsigned int *)mem;

  if (fifo->reader == -1)
    {
      scheduler_current(&fifo->reader);
    }

  len /= fifo->width;

  while (get < len)
    {
      IA32_SPIN_LOCK(fifo->lock);

      if (fifo->usage)
	{
	  memcpy(ptr, fifo->rptr, fifo->width * WORD_SIZE);
	  fifo->rptr += fifo->width;
	  fifo->offset += fifo->width;
	  if (fifo->rptr == fifo->end)
	    fifo->rptr = fifo->begin;
	  fifo->usage -= 1;
	  ++get;
	  ptr += fifo->width;
	}
      else
	{
	  //UNBLOCK(fifo->writer);
	  //YIELD();
	}

      IA32_SPIN_UNLOCK(fifo->lock);
    }
}

void	_dsx_mwmr_write(dsx_mwmr_t	fifo,
			unsigned char*	mem,
			unsigned int	len,
			const char*	file,
			const int	line)
{
  unsigned int	put = 0;
  unsigned int *ptr = (unsigned int *)mem;

  if (fifo->writer == -1)
    {
      scheduler_current(&fifo->writer);
    }

  len /= fifo->width;

  while (put < len)
    {
      IA32_SPIN_LOCK(fifo->lock);

      if (fifo->usage != fifo->depth)
	{
	  memcpy(fifo->wptr, ptr, fifo->width * WORD_SIZE);
	  fifo->wptr += fifo->width;
	  if (fifo->wptr == fifo->end)
	    fifo->wptr = fifo->begin;
	  fifo->usage += 1;
	  ++put;
	  ptr += fifo->width;
	}
      else
	{
	  //YIELD();
	}

      IA32_SPIN_UNLOCK(fifo->lock);
    }
}
