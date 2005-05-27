/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * region.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri May  6 00:03:48 2005   mycure
 */

#ifndef IA32_REGION_H
#define IA32_REGION_H

/*
 * region machdep macros
 */

#define		machdep_declare_region()				\
  extern t_machdep_region	machdep_region

#define		machdep_call_region(_function_, _args_...)		\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_region._function_ != NULL)				\
        machdep_region._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_region()				\
  /* is there anything here? */

/*
 * region machdep types
 */

typedef struct		s_machdep_region
{
  /* is there anything here? */
}			t_machdep_region;

/*
 * prototypes:     ../../../kaneton/machdep/region.c
 */

/*
 * eop
 */

#endif
