/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/region.h
 *
 * created       julien quintard   [wed dec 14 07:04:32 2005]
 * updated       julien quintard   [sat apr  1 22:58:17 2006]
 */

#ifndef IA32_CORE_REGION_H
#define IA32_CORE_REGION_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/region.h>
#include <klibc.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define REGION_VMEM_MAX		UINT_MAX

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_region()				\
  extern i_region		region_interface

#define		machdep_call_region(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (region_interface._function_ != NULL)				\
        _r_ = region_interface._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_region()

#define		machdep_data_o_region()

#endif
