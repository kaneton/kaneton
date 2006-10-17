/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/task.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:32:42 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:32:43 2006]
 */

#ifndef OCTANE_CORE_TASK_H
#define OCTANE_CORE_TASK_H	1

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
