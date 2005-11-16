/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:41 2005   mycure
 * last update   Tue Nov 15 22:11:30 2005   mycure
 */

#ifndef IA32_KANETON_AS_H
#define IA32_KANETON_AS_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/as.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the architecture dependent data for the as object
 */

typedef struct
{
  t_paddr			pd;
}				do_as;

/*
 * the as architecture dependent interface
 */

typedef struct
{
  t_error			(*as_clone)(t_asid,
					    t_asid);
  t_error			(*as_reserve)(t_asid*);
  t_error			(*as_release)(t_asid);
  t_error			(*as_init)(void);
  t_error			(*as_clean)(void);
}				i_as;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define		machdep_include_as()					\
  extern i_as			as_interface

#define		machdep_call_as(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (as_interface._function_ != NULL)				\
        _r_ = as_interface._function_(_args_);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_as()

#define		machdep_data_o_as()					\
  do_as				machdep

#endif
