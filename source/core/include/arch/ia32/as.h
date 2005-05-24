/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Thu May  5 23:42:19 2005   mycure
 */

#ifndef IA32_AS_H
#define IA32_AS_H

/*
 * as machdep macros
 */

#define		machdep_declare_as()					\
  extern t_machdep_as		machdep_as

#define		machdep_call_as(_function_, _args_...)			\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_as._function_ != NULL)				\
        machdep_as._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_as()					\
  /* is there anything here? */

/*
 * as machdep types
 */

typedef struct		s_machdep_as
{
  t_paddr		pd;
}			t_machdep_as;

/*
 * prototypes:     ../../../kaneton/machdep/as.c
 */

/*
 * eop
 */

#endif
