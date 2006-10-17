/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/timer.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:33:06 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:33:06 2006]
 */

#ifndef OCTANE_CORE_TIMER_H
#define OCTANE_CORE_TIMER_H	1



//typedef t_error			(*t_timer_handler)(void);

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_timer()					\
  extern d_timer		timer_dispatch

#define		machdep_call_timer(_function_, _args_...)		\
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

#define		machdep_data_m_timer()

#define		machdep_data_o_timer()

#endif
