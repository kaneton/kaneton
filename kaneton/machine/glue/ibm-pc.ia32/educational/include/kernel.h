/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ibm-pc.ia32/educational/include/kernel.h
 *
 * created       julien quintard   [sun jun  3 23:54:56 2007]
 * updated       julien quintard   [mon dec 13 11:14:10 2010]
 */

#ifndef GLUE_KERNEL_H
#define GLUE_KERNEL_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define		machine_include_kernel()				\
  extern d_kernel		glue_kernel_dispatch

#define		machine_call_kernel(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_kernel_dispatch.kernel_ ## _function_ != NULL)		\
        _r_ = glue_kernel_dispatch.kernel_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_kernel()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../kernel.c
 */

/*
 * eop
 */

#endif
