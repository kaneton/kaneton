/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/include/as.h
 *
 * created       julien quintard   [sun jun  3 23:54:56 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:42:29 2007]
 */

#ifndef GLUE_AS_H
#define GLUE_AS_H		1

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

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../as.c
 */

/*
 * eop
 */

#endif
