/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * sched.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri May  6 00:03:54 2005   mycure
 */

#ifndef IA32_SCHED_H
#define IA32_SCHED_H

/*
 * sched machdep macros
 */

#define		machdep_declare_sched()					\
  extern t_machdep_sched	machdep_sched

#define		machdep_call_sched(_function_, _args_...)		\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_sched._function_ != NULL)				\
        machdep_sched._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_sched()					\
  /* is there anything here? */

/*
 * sched machdep types
 */

typedef struct		s_machdep_sched
{
  /* is there anything here? */
}			t_machdep_sched;

/*
 * prototypes:     ../../../kaneton/machdep/sched.c
 */

/*
 * eop
 */

#endif
