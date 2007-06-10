/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/include/arch/ibm-pc.ia32-virtual/cpu.h
 *
 * created       julien quintard   [mon jun  4 01:05:31 2007]
 * updated       julien quintard   [fri jun  8 15:53:42 2007]
 */

#ifndef ARCHITECTURE_CPU_H
#define ARCHITECTURE_CPU_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_cpu()					\
  extern d_cpu		cpu_dispatch

#define		machdep_call_cpu(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (cpu_dispatch._function_ != NULL)				\
        _r_ = cpu_dispatch._function_(_args_);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_cpu()

#define		machdep_data_o_cpu()

#endif
