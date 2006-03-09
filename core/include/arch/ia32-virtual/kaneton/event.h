/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/include/arch/ia32-virtual/kaneton/event.h
 *
 * created       renaud voltz   [mon feb 13 00:56:13 2006]
 * updated       renaud voltz   [mon feb 13 00:56:13 2006]
 */

#ifndef IA32_KANETON_EVENT_H
#define IA32_KANETON_EVENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* #include <kaneton/event.h> */

/*
 * ---------- macros ----------------------------------------------------------
 */

#define		machdep_include_event()					\
  extern i_event		event_interface

#define		machdep_call_event(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (event_interface._function_ != NULL)				\
        _r_ = event_interface._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_event()

#define		machdep_data_o_event()

#endif
