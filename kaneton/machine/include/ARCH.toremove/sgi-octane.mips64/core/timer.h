/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/timer.h
 *
 * created       enguerrand raymond   [wed oct 18 14:19:58 2006]
 * updated       enguerrand raymond   [wed oct 18 14:20:07 2006]
 */

#ifndef MIPS64_CORE_TIMER_H
#define MIPS64_CORE_TIMER_H	1

//typedef t_error			(*t_timer_handler)(void);

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_timer()					\
  extern d_timer		timer_dispatch

#define		machine_call_timer(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (timer_dispatch._function_ != NULL)				\
        _r_ = timer_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_timer()

#define		machine_data_o_timer()

#endif
