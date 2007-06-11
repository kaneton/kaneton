/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-o2.mips64/core/event.h
 *
 * created       enguerrand raymond   [wed oct 18 14:16:28 2006]
 * updated       enguerrand raymond   [sun jan 28 18:19:45 2007]
 */

#ifndef MIPSR10K_CORE_EVENT_H
#define MIPSR10K_CORE_EVENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* #include <core/event.h> */

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_event()					\
  extern d_event		event_dispatch

#define		machine_call_event(_function_, _args_...)		\
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

#define		machine_data_m_event()

#define		machine_data_o_event()

#endif
