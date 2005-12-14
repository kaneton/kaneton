/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/arch/ia32/kaneton/region.h
 *
 * created       julien quintard   [wed dec 14 07:04:32 2005]
 * updated       julien quintard   [wed dec 14 07:32:07 2005]
 */

#ifndef IA32_KANETON_REGION_H
#define IA32_KANETON_REGION_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <kaneton/region.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the region architecture-dependent interface
 */

typedef struct
{
  t_error			(*region_clone)(t_asid,
						t_regid,
						t_regid*);
  t_error			(*region_reserve)(t_asid,
						  t_segid,
						  t_regid*);
  t_error			(*region_release)(t_regid);
  t_error			(*region_flush)(t_asid);
  t_error			(*region_init)(void);
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
