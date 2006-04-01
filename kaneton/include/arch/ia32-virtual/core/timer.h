/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/timer.h
 *
 * created       renaud voltz   [mon feb 13 00:56:13 2006]
 * updated       julien quintard   [sat apr  1 22:59:26 2006]
 */

#ifndef IA32_CORE_TIMER_H
#define IA32_CORE_TIMER_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_timer()					\
  extern i_timer		timer_interface

#define		machdep_call_timer(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (timer_interface._function_ != NULL)				\
        _r_ = timer_interface._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_timer()

#define		machdep_data_o_timer()

#endif
