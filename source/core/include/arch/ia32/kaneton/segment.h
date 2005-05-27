/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri May  6 00:04:03 2005   mycure
 */

#ifndef IA32_SEGMENT_H
#define IA32_SEGMENT_H

/*
 * segment machdep macros
 */

#define		machdep_declare_segment()				\
  extern t_machdep_segment	machdep_segment

#define		machdep_call_segment(_function_, _args_...)		\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_segment._function_ != NULL)				\
        machdep_segment._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_segment()				\
  /* is there anything here? */

/*
 *segment machdep types
 */

typedef struct		s_machdep_segment
{
  /* is there anything here? */
}			t_machdep_segment;

/*
 * prototypes:     ../../../kaneton/machdep/segment.c
 */

/*
 * eop
 */

#endif
