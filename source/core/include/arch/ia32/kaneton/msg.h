/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * msg.h
 * 
 * path          /home/mycure/kaneton/src/core/include/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri May  6 00:09:37 2005   mycure
 */

#ifndef IA32_MSG_H
#define IA32_MSG_H

/*
 * msg machdep macros
 */

#define		machdep_declare_msg()					\
  extern t_machdep_msg		machdep_msg

#define		machdep_call_msg(_function_, _args_...)			\
  (									\
    {									\
      int	_r_ = 0;						\
									\
      if (machdep_msg._function_ != NULL)				\
        machdep_msg._function_(_args_...);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_include_msg()					\
  /* is there anything here? */

/*
 * msg machdep types
 */

typedef struct		s_machdep_msg
{
  /* is there anything here? */
}			t_machdep_msg;

/*
 * prototypes:     ../../../kaneton/machdep/msg.c
 */

/*
 * eop
 */

#endif
