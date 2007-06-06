/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/include/arch/machdep/core/timer.h
 *
 * created       julien quintard   [wed jun  6 16:29:41 2007]
 * updated       julien quintard   [wed jun  6 16:31:18 2007]
 */

#ifndef GUARD_IA32_CORE_TIMER
#define GUARD_IA32_CORE_TIMER		1

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
