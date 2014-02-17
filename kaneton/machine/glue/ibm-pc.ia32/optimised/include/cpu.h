/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/include/cpu.h
 *
 * created       julien quintard   [mon jun  4 01:05:31 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:42:38 2007]
 */

#ifndef GLUE_CPU_H
#define GLUE_CPU_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_cpu()					\
  extern d_cpu		cpu_dispatch

#define		machine_call_cpu(_function_, _args_...)			\
  (									\
    {									\
      t_status _r_ = STATUS_OK;					\
									\
      if (cpu_dispatch._function_ != NULL)				\
        _r_ = cpu_dispatch._function_(_args_);				\
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

t_status		glue_cpu_current(i_cpu*			cpuid);


/*
 * eop
 */

#endif
