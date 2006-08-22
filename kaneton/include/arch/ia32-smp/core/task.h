/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/task.h
 *
 * created       julien quintard   [sat dec 10 15:24:34 2005]
 * updated       matthieu bucchianeri   [sat jul 29 18:36:24 2006]
 */

#ifndef IA32_CORE_TASK_H
#define IA32_CORE_TASK_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/task.h>

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

#endif
