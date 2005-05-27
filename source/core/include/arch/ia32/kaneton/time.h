/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * time.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Thu May  5 23:41:36 2005   mycure
 */

#ifndef IA32_TIME_H
#define IA32_TIME_H

/*
 * time machdep macros
 */

#define		machdep_declare_time()					\
  extern t_machdep_time		machdep_time

#define		machdep_call_time(_function_, _args_...)		\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_time._function_ != NULL)				\
        machdep_time._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_time()					\
  /* is there anything here? */

/*
 * time machdep types
 */

typedef struct		s_machdep_time
{
  /* is there anything here? */
}			t_machdep_time;

/*
 * prototypes:     ../../../kaneton/machdep/time.c
 */

/*
 * eop
 */

#endif
