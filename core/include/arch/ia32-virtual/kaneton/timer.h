/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/include/arch/ia32-virtual/kaneton/timer.h
 *
 * created       renaud voltz   [mon feb 13 00:56:13 2006]
 * updated       renaud voltz   [mon feb 13 00:56:13 2006]
 */

#ifndef IA32_KANETON_TIMER_H
#define IA32_KANETON_TIMER_H	1

/*
 * ---------- macros ----------------------------------------------------------
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
