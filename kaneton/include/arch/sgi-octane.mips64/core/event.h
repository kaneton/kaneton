/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/event.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:30:41 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:30:42 2006]
 */

#ifndef OCTANE_CORE_EVENT_H
#define OCTANE_CORE_EVENT_H	1

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
