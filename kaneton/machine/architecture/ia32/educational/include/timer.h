/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...include/arch/ibm-pc.ia32-virtual/timer.h
 *
 * created       julien quintard   [wed jun  6 16:29:41 2007]
 * updated       julien quintard   [thu jun  7 15:33:53 2007]
 */

#ifndef ARCHITECTURE_TIMER_H
#define ARCHITECTURE_TIMER_H		1

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
