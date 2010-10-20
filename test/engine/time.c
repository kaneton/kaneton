/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/test/engine/time.c
 *
 * created       julien quintard   [mon oct 18 09:06:03 2010]
 * updated       julien quintard   [mon oct 18 09:29:29 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "engine.h"

/*
 * ---------- functions -------------------------------------------------------
 */

t_uint8			test_engine_time_bcd2bin(t_uint8		c)
{
  t_uint8		result;
  t_uint8		x = (c & 0xF0) >> 4;

  result = 10 * x;
  x = c & 0x0F;
  result += x;

  return (result);
}

void			test_engine_time_start(t_test_engine_cell*	cell)
{
  t_uint8		i;

  OUTB(0x70, 0);
  INB(0x71, i);

  cell->time.start.stamp = test_engine_time_bcd2bin(i);
}

void			test_engine_time_stop(t_test_engine_cell*	cell)
{
  t_uint8		i;

  OUTB(0x70, 0);
  INB(0x71, i);

  cell->time.stop.stamp = test_engine_time_bcd2bin(i);
}
