/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/as.h
 *
 * created       enguerrand raymond   [wed oct 18 14:14:55 2006]
 * updated       enguerrand raymond   [wed oct 18 14:15:03 2006]
 */

#ifndef MIPS64_CORE_AS_H
#define MIPS64_CORE_AS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/as.h>

/*
 * ---------- types -----------------------------------------------------------
 */

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

#define		machdep_data_o_as()

#endif
