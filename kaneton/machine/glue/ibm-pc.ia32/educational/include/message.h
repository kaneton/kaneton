/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...bm-pc.ia32/educational/include/message.h
 *
 * created       julien quintard   [wed jun  6 16:20:48 2007]
 * updated       julien quintard   [sun dec 19 13:46:24 2010]
 */

#ifndef GLUE_MESSAGE_H
#define GLUE_MESSAGE_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the message dispatcher.
 */

#define		machine_include_message()				\
  extern d_message	glue_message_dispatch

/*
 * this macro-function dispatches the message calls.
 */

#define		machine_call_message(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_message_dispatch.message_ ## _function_ != NULL)		\
        _r_ = glue_message_dispatch.message_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_message'.
 */

#define		machine_data_m_message()

/*
 * this macro-function includes data in 'o_message'.
 */

#define		machine_data_o_message()

/*
 * this macro-function includes data in 'o_message_type'.
 */

#define		machine_data_o_message_type()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../message.c
 */

/*
 * ../message.c
 */


/*
 * eop
 */

#endif
