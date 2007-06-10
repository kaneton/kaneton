/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/region.h
 *
 * created       enguerrand raymond   [wed oct 18 14:17:43 2006]
 * updated       enguerrand raymond   [wed oct 18 14:17:50 2006]
 */

#ifndef MIPS64_CORE_REGION_H
#define MIPS64_CORE_REGION_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/region.h>
//#include <libc.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define REGION_VMEM_MIN		PAGESZ
#define REGION_VMEM_MAX		UINT_MAX

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_region()				\
  extern d_region		region_dispatch

#define		machdep_call_region(_function_, _args_...)		\
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

#define		machdep_data_m_region()

#define		machdep_data_o_region()

#endif
