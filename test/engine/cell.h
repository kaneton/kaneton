/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/test/engine/cell.h
 *
 * created       julien quintard   [mon oct 18 09:19:01 2010]
 * updated       julien quintard   [mon oct 18 09:37:34 2010]
 */

#ifndef TEST_ENGINE_CELL_H
#define TEST_ENGINE_CELL_H	1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- structures ------------------------------------------------------
 */

typedef struct
{
  t_uint32		stamp;
}			t_test_engine_time;

typedef struct
{
  t_uint32		nalloc;
  t_uint32		nfree;
}			t_test_engine_leak;

typedef struct
{
  struct
  {
    t_test_engine_time	start;
    t_test_engine_time	stop;
  }			time;

  struct
  {
    t_test_engine_leak	start;
    t_test_engine_leak	stop;
  }			leak;
}			t_test_engine_cell;

#endif
