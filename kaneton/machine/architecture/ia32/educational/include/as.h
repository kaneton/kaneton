/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/include/arch/ibm-pc.ia32-virtual/as.h
 *
 * created       julien quintard   [sun jun  3 23:54:56 2007]
 * updated       julien quintard   [thu jun  7 15:33:08 2007]
 */

#ifndef ARCHITECTURE_AS_H
#define ARCHITECTURE_AS_H		1

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
  struct								\
  {									\
    t_ia32_directory		pd;					\
  }				machdep;

#endif
