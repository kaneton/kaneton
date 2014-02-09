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
 * updated       julien quintard   [sun dec 19 13:43:00 2010]
 */

#ifndef GLUE_KERNEL_H
#define GLUE_KERNEL_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the kernel dispatcher.
 */

#define		machine_include_kernel()				\
  extern d_kernel		glue_kernel_dispatch

/*
 * this macro-functio dispatches the kernel calls.
 */

#define		machine_call_kernel(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_kernel_dispatch.kernel_ ## _function_ != NULL)		\
        _r_ = glue_kernel_dispatch.kernel_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_kernel'.
 */

#define		machine_data_m_kernel()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../kernel.c
 */

/*
 * ../kernel.c
 */


/*
 * eop
 */

#endif
