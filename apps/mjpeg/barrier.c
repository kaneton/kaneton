/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/barrier.c
 *
 * created       matthieu bucchianeri   [sat dec  2 01:57:55 2006]
 * updated       matthieu bucchianeri   [sun dec  3 19:59:30 2006]
 */

#include "dsx.h"

void	dsx_barrier_alloc(dsx_barrier_t*	barp,
			  int			nconn)
{
  *barp = malloc(sizeof (dsx_barrier_t));

  (*barp)->c = (*barp)->_c = nconn;

  if (set_reserve(array, SET_OPT_NONE, nconn, sizeof(i_thread), &(*barp)->threads) != ERROR_NONE)
    {
      printf("panic !\n");
      while (1)
	;
    }
}

void	dsx_barrier_reset(dsx_barrier_t		bar)
{
  t_iterator	i;
  t_state	st;
  i_thread	*th;

  CLI();
  set_foreach(SET_OPT_FORWARD, bar->threads, &i, st)
    {
      if (set_object(bar->threads, i, (void**)&th) != ERROR_NONE)
	thread_state(*th, SCHED_STATE_RUN);
    }
  set_flush(bar->threads);
  bar->c = bar->_c;
  STI();
}

void	dsx_barrier_wait(dsx_barrier_t		bar)
{
  i_thread	th;
  t_uint8	b = 0;

  CLI();

  sched_current(&th);

  bar->c--;

  if (bar->c == 0)
    {
      dsx_barrier_reset(bar);
      b = 1;
    }

  STI();
  if (!b)
    thread_state(th, SCHED_STATE_STOP);
}
