/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../include/arch/ibm-pc.ia32-virtual/task.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [thu jun  7 15:36:25 2007]
 */

#ifndef ARCHITECTURE_TASK_H
#define ARCHITECTURE_TASK_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_task()					\
  extern d_task			task_dispatch

#define		machdep_call_task(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (task_dispatch._function_ != NULL)				\
        _r_ = task_dispatch._function_(_args_);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_task()

#define		machdep_data_o_task()					\
  struct								\
  {									\
    t_uint8			iomap[8192];				\
  }				machdep;

#endif
