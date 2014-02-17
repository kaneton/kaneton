/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ue/ibm-pc.ia32/educational/include/cpu.h
 *
 * created       julien quintard   [mon jun  4 01:05:31 2007]
 * updated       julien quintard   [sun dec 19 13:38:26 2010]
 */

#ifndef GLUE_CPU_H
#define GLUE_CPU_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the CPU dispatcher.
 */

#define		machine_include_cpu()					\
  extern d_cpu		glue_cpu_dispatch

/*
 * this macro-function dispatches the CPU calls.
 */

#define		machine_call_cpu(_function_, _args_...)			\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_cpu_dispatch.cpu_ ## _function_ != NULL)			\
        _r_ = glue_cpu_dispatch.cpu_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_cpu'.
 */

#define		machine_data_m_cpu()

/*
 * this macro-function includes data in 'o_cpu'.
 */

#define		machine_data_o_cpu()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../cpu.c
 */

/*
 * ../cpu.c
 */

t_status		glue_cpu_current(i_cpu*			cpu);


/*
 * eop
 */

#endif
