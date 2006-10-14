/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/event.h
 *
 * created       renaud voltz   [mon feb 13 00:56:13 2006]
 * updated       julien quintard   [sun apr  2 14:09:53 2006]
 */

#ifndef IA32_CORE_EVENT_H
#define IA32_CORE_EVENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* #include <core/event.h> */

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_event()					\
  extern d_event		event_dispatch

#define		machdep_call_event(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (event_dispatch._function_ != NULL)				\
        _r_ = event_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_event()

#define		machdep_data_o_event()

#endif
