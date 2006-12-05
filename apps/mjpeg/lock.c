/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/lock.c
 *
 * created       matthieu bucchianeri   [sat dec  2 01:59:29 2006]
 * updated       matthieu bucchianeri   [tue dec  5 23:24:49 2006]
 */

#include "dsx.h"

void	dsx_lock_alloc(dsx_lock_t*	lockp)
{
  *lockp = malloc(sizeof (dsx_lock_t));

  (*lockp)->v = 0;
}

void	dsx_lock_lock(dsx_lock_t	lock)
{
  while (1)
    {
      CLI();
      if (lock->v == 0)
	{
	  lock->v = 1;
	  break;
	}
      STI();
    }
}

void	dsx_lock_unlock(dsx_lock_t	lock)
{
  CLI();
  lock->v = 0;
  STI();
}
