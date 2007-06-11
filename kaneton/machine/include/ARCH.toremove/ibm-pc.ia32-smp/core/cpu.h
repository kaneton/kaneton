/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/cpu.h
 *
 * created       matthieu bucchianeri   [sat jul 29 18:09:38 2006]
 * updated       matthieu bucchianeri   [sat jul 29 18:10:03 2006]
 */

#ifndef IA32_CORE_CPU_H
#define IA32_CORE_CPU_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <core/cpu.h>*/

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_cpu()					\
  extern d_cpu		cpu_dispatch

#define		machine_call_cpu(_function_, _args_...)			\
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

#define		machine_data_m_cpu()

#define		machine_data_o_cpu()

#endif
