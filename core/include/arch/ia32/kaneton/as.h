/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:41 2005   mycure
 * last update   Tue Aug 30 16:10:28 2005   mycure
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
}				d_as;

/*
 * the as architecture dependent interface
 */

typedef struct
{
  t_error			(*as_dump)(void);
  t_error			(*as_rsv)(t_asid*);
  t_error			(*as_rel)(t_asid);
  t_error			(*as_get)(t_asid, o_as**);
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
      t_error	_r_ = ERROR_UNKNOWN;					\
									\
      if (as_interface._function_ != NULL)				\
        _r_ = as_interface._function_((_args_));			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_as()					\
  d_as				machdep

#endif
