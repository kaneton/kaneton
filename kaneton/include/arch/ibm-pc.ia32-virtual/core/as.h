/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/machdep/core/as.h
 *
 * created       julien quintard   [sun jun  3 23:54:56 2007]
 * updated       julien quintard   [wed jun  6 16:12:43 2007]
 */

#ifndef GUARD_IA32_CORE_AS
#define GUARD_IA32_CORE_AS		1

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

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libia32.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the architecture dependent data for the as object
 */

typedef struct
{
  t_ia32_directory		pd;
}				ao_as;

#endif
