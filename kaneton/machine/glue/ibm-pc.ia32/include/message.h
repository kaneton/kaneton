/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/include/message.h
 *
 * created       julien quintard   [wed jun  6 16:20:48 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:23:31 2007]
 */

#ifndef GLUE_MESSAGE_H
#define GLUE_MESSAGE_H		1

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

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../message.c
 */

/*
 * ../message.c
 */

t_error		glue_message_init(void);

t_error		glue_message_clean(void);


/*
 * eop
 */

#endif
