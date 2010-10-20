/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/test/engine/cell.c
 *
 * created       julien quintard   [mon oct 18 09:31:05 2010]
 * updated       julien quintard   [mon oct 18 09:38:29 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "engine.h"

/*
 * ---------- functions -------------------------------------------------------
 */

void		test_engine_cell_start(t_test_engine_cell*	cell)
{
  test_engine_time_start(cell);
  test_engine_leak_start(cell);
}

void		test_engine_cell_stop(t_test_engine_cell*	cell)
{
  test_engine_leak_stop(cell);
  test_engine_time_stop(cell);
}
