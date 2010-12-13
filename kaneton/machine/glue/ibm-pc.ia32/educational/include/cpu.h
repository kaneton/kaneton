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
 * updated       julien quintard   [fri dec 10 21:14:28 2010]
 */

#ifndef GLUE_CPU_H
#define GLUE_CPU_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define		machine_include_cpu()					\
  extern d_cpu		glue_cpu_dispatch

#define		machine_call_cpu(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_cpu_dispatch.cpu_ ## _function_ != NULL)			\
        _r_ = glue_cpu_dispatch.cpu_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_cpu()

#define		machine_data_o_cpu()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../cpu.c
 */

/*
 * ../cpu.c
 */

t_error			glue_cpu_current(i_cpu*			cpu);


/*
 * eop
 */

#endif
