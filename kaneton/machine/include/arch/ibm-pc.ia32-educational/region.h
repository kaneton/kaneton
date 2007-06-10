/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...nclude/arch/ibm-pc.ia32-virtual/region.h
 *
 * created       julien quintard   [wed jun  6 16:22:05 2007]
 * updated       julien quintard   [thu jun  7 15:32:35 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file supposes it is included by core.h so that PAGESZ is defined.
 */

#ifndef ARCHITECTURE_REGION_H
#define ARCHITECTURE_REGION_H		1

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
 * ---------- macros ----------------------------------------------------------
 */

#define REGION_VMEM_MIN		PAGESZ
#define REGION_VMEM_MAX		0xffffffffU

#endif
