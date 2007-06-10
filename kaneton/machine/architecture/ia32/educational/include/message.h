/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...clude/arch/ibm-pc.ia32-virtual/message.h
 *
 * created       julien quintard   [wed jun  6 16:20:48 2007]
 * updated       julien quintard   [thu jun  7 15:40:41 2007]
 */

#ifndef ARCHITECTURE_MESSAGE_H
#define ARCHITECTURE_MESSAGE_H		1

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
