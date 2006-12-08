/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/mwmr.c
 *
 * created       matthieu bucchianeri   [sat dec  2 02:00:48 2006]
 * updated       matthieu bucchianeri   [fri dec  8 01:49:49 2006]
 */

#include "dsx.h"

#define WORD_SIZE 4

void	dsx_mwmr_alloc(dsx_mwmr_t*	fifop,
		       unsigned int	w,
		       unsigned int	depth,
		       const char*	name)
{
  *fifop = malloc(sizeof (**fifop));
  (*fifop)->offset = 0;
  (*fifop)->usage = 0;
  (*fifop)->name = name;
  (*fifop)->begin = (*fifop)->rptr = (*fifop)->wptr = malloc(w * depth * WORD_SIZE);
  (*fifop)->end = (*fifop)->begin + w * depth;
  (*fifop)->width = w;
  (*fifop)->depth = depth;
}

void	_dsx_mwmr_reset(dsx_mwmr_t	fifo,
			const char*	file,
			const int	line)
{
  CLI();

  fifo->usage = 0;
  fifo->rptr = fifo->wptr = fifo->begin;

  STI();
}

void	_dsx_mwmr_read(dsx_mwmr_t	fifo,
		       unsigned char*	mem,
		       unsigned int	len,
		       const char*	file,
		       const int	line)
{
  unsigned int	get = 0;
  unsigned int *ptr = (unsigned int *)mem;

  len /= fifo->width;

  while (get < len)
    {
      CLI();

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

      STI();
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

  len /= fifo->width;

  while (put < len)
    {
      CLI();

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

      STI();
    }
}
