/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * trap.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Thu May  5 23:41:57 2005   mycure
 */

#ifndef IA32_TRAP_H
#define IA32_TRAP_H

/*
 * trap machdep macros
 */

#define		machdep_declare_trap()					\
  extern t_machdep_trap		machdep_trap

#define		machdep_call_trap(_function_, _args_...)		\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_trap._function_ != NULL)				\
        machdep_trap._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_trap()					\
  /* is there anything here? */

/*
 * trap machdep types
 */

typedef struct		s_machdep_trap
{
  /* is there anything here? */
}			t_machdep_trap;

/*
 * prototypes:     ../../../kaneton/machdep/trap.c
 */

/*
 * eop
 */

#endif
