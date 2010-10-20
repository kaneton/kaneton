/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/test/engine/engine.h
 *
 * created       julien quintard   [sun oct 17 15:43:58 2010]
 * updated       julien quintard   [mon oct 18 09:31:00 2010]
 */

#ifndef TEST_ENGINE_ENGINE_H
#define TEST_ENGINE_ENGINE_H	1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "cell.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_test_engine_cell	cell;

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define TEST_ENTER()							\
  test_engine_cell_start(&cell);

#define TEST_LEAVE()							\
  test_engine_cell_stop(&cell);						\
  return;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      leak.c
 *      time.c
 *      task.c
 *      thread.c
 *      cell.c
 */

/*
 * leak.c
 */

void			test_engine_leak_start(t_test_engine_cell*	cell);

void			test_engine_leak_stop(t_test_engine_cell*	cell);


/*
 * time.c
 */

t_uint8			test_engine_time_bcd2bin(t_uint8		c);

void			test_engine_time_start(t_test_engine_cell*	cell);

void			test_engine_time_stop(t_test_engine_cell*	cell);


/*
 * task.c
 */

t_error			test_engine_task_spawn(t_class		class,
					       i_task*		task);


/*
 * thread.c
 */

int		test_engine_thread_spawn(i_task			task,
					 t_prior		prior,
					 t_vaddr		entry,
					 i_thread*		thread);


/*
 * cell.c
 */

void		test_engine_cell_start(t_test_engine_cell*	cell);

void		test_engine_cell_stop(t_test_engine_cell*	cell);


/*
 * eop
 */

#endif
