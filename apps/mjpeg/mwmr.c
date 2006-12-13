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

#include <klibc.h>
#include <kaneton.h>

#include "dsx.h"

#define WORD_SIZE 4

extern i_task mjpeg_task;

#if 1
#define WAIT()		pthread_yield()

#define SIGNAL()

#define YIELD()		pthread_yield()
#endif

#if 0
#define WAIT()		asm volatile("int $29")

#define SIGNAL()	asm volatile("int $30")

#define YIELD()		pthread_yield();
#endif

void	dsx_mwmr_alloc(dsx_mwmr_t	fifo,
		       unsigned int	w,
		       unsigned int	depth,
		       const char*	name)
{

  t_vaddr addr;

  if (map_reserve(mjpeg_task, MAP_OPT_NONE | MAP_OPT_USER, 4096, PERM_READ | PERM_WRITE ,&addr) != ERROR_NONE)
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
}

void	_dsx_mwmr_reset(dsx_mwmr_t	fifo,
			const char*	file,
			const int	line)
{
  CLI();

  fifo->usage = 0;
  fifo->rptr = fifo->wptr = fifo->begin;
  SIGNAL();

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
      else
	WAIT();


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
	  SIGNAL();
	}
      else
	YIELD();

      STI();

    }
}
