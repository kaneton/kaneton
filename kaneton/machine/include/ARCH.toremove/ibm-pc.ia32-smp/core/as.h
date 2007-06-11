/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ibm-pc.ia32-virtual/core/as.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       matthieu bucchianeri   [tue feb  6 23:18:51 2007]
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
  t_ia32_directory		pd;
}				ao_as;

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_as()					\
  extern d_as			as_dispatch

#define		machine_call_as(_function_, _args_...)			\
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

#define		machine_data_m_as()

#define		machine_data_o_as()					\
  ao_as				machdep

#endif
