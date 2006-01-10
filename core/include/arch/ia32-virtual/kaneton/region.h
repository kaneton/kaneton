/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/arch/ia32-virtual/kaneton/region.h
 *
 * created       julien quintard   [wed dec 14 07:04:32 2005]
 * updated       matthieu bucchianeri   [tue jan 10 00:38:10 2006]
 */

#ifndef IA32_KANETON_REGION_H
#define IA32_KANETON_REGION_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <kaneton/region.h>
#include <klibc.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define REGION_VMEM_MAX		UINT_MAX

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the region architecture-dependent interface
 */

typedef struct
{
  t_error			(*region_reserve)(t_asid,
						  t_segid,
						  t_opts,
						  t_vaddr,
						  t_regid*);
  t_error			(*region_release)(t_asid,
						  t_regid);
  t_error			(*region_flush)(t_asid);
  t_error			(*region_init)(t_fit,
					       t_vaddr,
					       t_vsize);
  t_error			(*region_clean)(void);
}				i_region;

/*
 * ---------- macros ----------------------------------------------------------
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
