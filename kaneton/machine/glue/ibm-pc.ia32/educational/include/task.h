/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/ibm-pc.ia32/educational/include/task.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [sun jan  9 22:28:23 2011]
 */

#ifndef GLUE_TASK_H
#define GLUE_TASK_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the task dispatcher.
 */

#define		machine_include_task()					\
  extern d_task		glue_task_dispatch

/*
 * this macro-function dispatches the task calls.
 */

#define		machine_call_task(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_task_dispatch.task_ ## _function_ != NULL)		\
        _r_ = glue_task_dispatch.task_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_task'.
 */

#define		machine_data_m_task()

/*
 * this macro-function includes data in the task object i.e 'o_task'.
 *
 * the embedded data include (i) the I/O map and (ii) a boolean indicating
 * if the I/O map must be flushed next time the task is scheduled.
 */

#define		machine_data_o_task()					\
  struct								\
  {									\
    struct								\
    {									\
      t_uint8			map[ARCHITECTURE_IO_MAP_SIZE];		\
      t_boolean			flush;					\
    }				io;					\
  }				machine;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../task.c
 */

/*
 * ../task.c
 */

t_status		glue_task_show(i_task			id,
				       mt_margin		margin);

t_status		glue_task_reserve(t_class		class,
					  t_behaviour		behav,
					  t_priority		prior,
					  i_task*		id);


/*
 * eop
 */

#endif
