/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/include/arch/ia32-virtual/kaneton/as.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       matthieu bucchianeri   [fri mar  3 13:24:54 2006]
 */

#ifndef IA32_KANETON_AS_H
#define IA32_KANETON_AS_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <kaneton/as.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the architecture dependent data for the as object
 */

typedef struct
{
/*                                                                [cut] k2   */

  t_directory			pd;

/*                                                               [cut] /k2   */
}				do_as;

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
