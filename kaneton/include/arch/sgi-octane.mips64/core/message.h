/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/message.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:31:21 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:31:21 2006]
 */

#ifndef OCTANE_CORE_MESSAGE_H
#define OCTANE_CORE_MESSAGE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <core/message.h>*/

/*
 * ---------- types -----------------------------------------------------------
 */

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

#endif
