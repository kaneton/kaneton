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
 * updated       julien quintard   [wed apr 15 07:23:07 2009]
 */

#ifndef GLUE_TASK_H
#define GLUE_TASK_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_task()					\
  extern d_task			task_dispatch

#define		machine_call_task(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;					\
									\
      if (task_dispatch._function_ != NULL)				\
        _r_ = task_dispatch._function_(_args_);				\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_task()

#define		machine_data_o_task()					\
  struct								\
  {									\
    t_uint8			iomap[8192];				\
    t_uint8			ioflush;				\
  }				machine;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../task.c
 */

/*
 * ../task.c
 */

t_status		glue_task_clone(i_task			old,
					i_task*			new);

t_status		glue_task_reserve(t_class			class,
					  t_behav			behav,
					  t_prior			prior,
					  i_task*			id);

t_status		glue_task_initialize(void);


/*
 * eop
 */

#endif
