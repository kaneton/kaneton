/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/mwmr.c
 *
 * created       matthieu bucchianeri   [sat dec  2 02:00:48 2006]
 * updated       matthieu bucchianeri   [sun dec  3 20:21:49 2006]
 */

#include "dsx.h"

void	dsx_mwmr_alloc(dsx_mwmr_t*	fifop,
		       unsigned int	w,
		       unsigned int	depth,
		       const char*	name)
{
  *fifop = malloc(sizeof (dsx_mwmr_t));

  if (set_reserve(pipe, SET_OPT_NONE, w, &(*fifop)->fifo) != ERROR_NONE)
    {
      printf("panic !\n");
      while (1)
	;
    }
  (*fifop)->w = w;
}

void	_dsx_mwmr_reset(dsx_mwmr_t	fifo,
			const char*	file,
			const int	line)
{
  set_flush(fifo->fifo);
}

void	_dsx_mwmr_read(dsx_mwmr_t	fifo,
		       unsigned char*	mem,
		       unsigned int	len,
		       const char*	file,
		       const int	line)
{
  unsigned int	get = 0;
  unsigned char *p;

  while (get < len)
    {
      CLI();

      if (set_pick(fifo->fifo, (void**)&p) == ERROR_NONE)
	{
	  memcpy(mem, p, fifo->w);
	  set_pop(fifo->fifo);

	  mem += fifo->w;
	  get += fifo->w;
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

  CLI();
  while (put < len)
    {
      set_push(fifo->fifo, mem);

      mem += fifo->w;
      put += fifo->w;
    }
  STI();
}
