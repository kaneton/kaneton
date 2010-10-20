/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/test/engine/leak.c
 *
 * created       julien quintard   [sun oct 17 15:40:07 2010]
 * updated       julien quintard   [mon oct 18 09:24:13 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "engine.h"

/*
 * ---------- functions -------------------------------------------------------
 */

void			test_engine_leak_start(t_test_engine_cell*	cell)
{
  cell->leak.start.nalloc = alloc_nalloc();
  cell->leak.start.nfree = alloc_nfree();
}

void			test_engine_leak_stop(t_test_engine_cell*	cell)
{
  cell->leak.stop.nalloc = alloc_nalloc();
  cell->leak.stop.nfree = alloc_nfree();
}
