/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/include/arch/machdep/core/task.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [wed jun  6 16:26:41 2007]
 */

#ifndef GUARD_IA32_CORE_TASK
#define GUARD_IA32_CORE_TASK		1

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
  ao_task				machdep

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/core.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the architecture dependent data for the task object
 */

typedef struct
{
  t_uint8			iomap[8192];
}				ao_task;

#endif
