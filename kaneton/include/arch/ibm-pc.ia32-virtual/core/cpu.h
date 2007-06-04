/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/machdep/core/cpu.h
 *
 * created       julien quintard   [mon jun  4 01:05:31 2007]
 * updated       julien quintard   [mon jun  4 01:10:00 2007]
 */

#ifndef IA32_CORE_CPU_H
#define IA32_CORE_CPU_H	1

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

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/cpu.h>

#endif
