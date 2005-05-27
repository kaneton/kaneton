/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * thread.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri May  6 00:04:23 2005   mycure
 */

#ifndef IA32_THREAD_H
#define IA32_THREAD_H

/*
 * thread machdep macros
 */

#define		machdep_declare_thread()				\
  extern t_machdep_thread	machdep_thread

#define		machdep_call_thread(_function_, _args_...)		\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_thread._function_ != NULL)				\
        machdep_thread._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_thread()				\
  /* is there anything here? */

/*
 * thread machdep types
 */

typedef struct		s_machdep_thread
{
  /* is there anything here? */
}			t_machdep_thread;

/*
 * prototypes:     ../../../kaneton/machdep/thread.c
 */

/*
 * eop
 */

#endif
