/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/as.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:23:07 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 14:00:51 2006]
 */

#ifndef OCTANE_CORE_AS_H
#define OCTANE_CORE_AS_H		1

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
