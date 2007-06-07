/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...re/include/arch/ibm-pc.ia32-virtual/as.h
 *
 * created       julien quintard   [sun jun  3 23:54:56 2007]
 * updated       julien quintard   [thu jun  7 12:06:12 2007]
 */

#ifndef ARCHITECTURE_AS_H
#define ARCHITECTURE_AS_H		1

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
  struct								\
  {									\
    t_ia32_directory		pd;					\
  }				ao_as;					\

#endif
