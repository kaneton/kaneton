/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/task.h
 *
 * created       julien quintard   [sat dec 10 15:24:34 2005]
 * updated       julien quintard   [sat apr  1 22:59:15 2006]
 */

#ifndef IA32_CORE_TASK_H
#define IA32_CORE_TASK_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/task.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_task()					\
  extern i_task			task_interface

#define		machdep_call_task(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (task_interface._function_ != NULL)				\
        _r_ = task_interface._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_task()

#define		machdep_data_o_task()

#endif
