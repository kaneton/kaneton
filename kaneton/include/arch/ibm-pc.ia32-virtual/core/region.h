/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...neton/include/arch/machdep/core/region.h
 *
 * created       julien quintard   [wed jun  6 16:22:05 2007]
 * updated       julien quintard   [wed jun  6 16:23:59 2007]
 */

#ifndef GUARD_IA32_CORE_REGION
#define GUARD_IA32_CORE_REGION		1

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

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/core/core.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define REGION_VMEM_MIN		PAGESZ
#define REGION_VMEM_MAX		0xffffffffU

#endif
