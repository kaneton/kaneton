/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * cpu.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri May  6 00:08:37 2005   mycure
 */

#ifndef IA32_CPU_H
#define IA32_CPU_H

/*
 * cpu machdep macros
 */

#define		machdep_declare_cpu()					\
  extern t_machdep_as		machdep_as

#define		machdep_call_cpu(_function_, _args_...)			\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_cpu._function_ != NULL)				\
        machdep_cpu._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_cpu()					\
  /* is there anything here? */

/*
 * cpu machdep types
 */

typedef struct		s_machdep_cpu
{
  /* is there anything here? */
}			t_machdep_cpu;

/*
 * prototypes:     ../../../kaneton/machdep/cpu.c
 */

/*
 * eop
 */

#endif
