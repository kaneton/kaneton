/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/as.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       matthieu bucchianeri   [mon apr  3 00:02:10 2006]
 */

#ifndef IA32_CORE_AS_H
#define IA32_CORE_AS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/as.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the architecture dependent data for the as object
 */

typedef struct
{
/*                                                                [cut] k2   */

  t_ia32_directory		pd;

/*                                                               [cut] /k2   */
}				ao_as;

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_as()					\
  extern d_as			as_dispatch

#define		machdep_call_as(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (as_dispatch._function_ != NULL)				\
        _r_ = as_dispatch._function_(_args_);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_as()

#define		machdep_data_o_as()					\
  ao_as				machdep

#endif
