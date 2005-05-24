/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * task.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri May  6 00:04:08 2005   mycure
 */

#ifndef IA32_TASK_H
#define IA32_TASK_H

/*
 * task machdep macros
 */

#define		machdep_declare_task()					\
  extern t_machdep_task		machdep_task

#define		machdep_call_task(_function_, _args_...)		\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_task._function_ != NULL)				\
        machdep_task._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_task()					\
  /* is there anything here? */

/*
 * task machdep types
 */

typedef struct		s_machdep_task
{
  /* is there anything here? */
}			t_machdep_task;

/*
 * prototypes:     ../../../kaneton/machdep/task.c
 */

/*
 * eop
 */

#endif
