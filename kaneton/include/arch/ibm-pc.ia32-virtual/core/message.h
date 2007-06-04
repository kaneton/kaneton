/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/message.h
 *
 * created       matthieu bucchianeri   [sat jun 17 19:41:39 2006]
 * updated       matthieu bucchianeri   [sat jun 17 19:45:46 2006]
 */

#ifndef IA32_CORE_MESSAGE_H
#define IA32_CORE_MESSAGE_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_message()				\
  extern d_message		message_dispatch

#define		machdep_call_message(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (message_dispatch._function_ != NULL)				\
        _r_ = message_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_message()

#define		machdep_data_o_message()

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <core/message.h>

#endif
