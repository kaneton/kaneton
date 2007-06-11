/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/region.h
 *
 * created       julien quintard   [wed dec 14 07:04:32 2005]
 * updated       matthieu bucchianeri   [tue apr 11 16:16:58 2006]
 */

#ifndef IA32_CORE_REGION_H
#define IA32_CORE_REGION_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/region.h>
#include <libc.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define REGION_VMEM_MIN		PAGESZ
#define REGION_VMEM_MAX		UINT_MAX

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_region()				\
  extern d_region		region_dispatch

#define		machine_call_region(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (region_dispatch._function_ != NULL)				\
        _r_ = region_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_region()

#define		machine_data_o_region()

#endif
