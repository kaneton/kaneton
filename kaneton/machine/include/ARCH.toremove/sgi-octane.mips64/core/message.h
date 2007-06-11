/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/message.h
 *
 * created       enguerrand raymond   [wed oct 18 14:17:26 2006]
 * updated       enguerrand raymond   [wed oct 18 14:17:33 2006]
 */

#ifndef MIPS64_CORE_MESSAGE_H
#define MIPS64_CORE_MESSAGE_H	1

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

#define		machine_include_message()				\
  extern d_message		message_dispatch

#define		machine_call_message(_function_, _args_...)		\
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

#define		machine_data_m_message()

#define		machine_data_o_message()

#endif
