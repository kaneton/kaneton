/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...chine/glue/ibm-pc.ia32/include/message.h
 *
 * created       julien quintard   [wed jun  6 16:20:48 2007]
 * updated       matthieu bucchianeri   [mon jul 23 12:40:53 2007]
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
      t_status _r_ = STATUS_OK;					\
									\
      if (message_dispatch._function_ != NULL)				\
        _r_ = message_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_message()

#define		machine_data_o_message()

#define		machine_data_o_message_type()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../message.c
 */

/*
 * ../message.c
 */

t_status		glue_message_return(i_thread		thread,
					    t_status	code);

t_status		glue_message_return_info(i_thread	thread,
						 t_statuscode,
						 t_vsize	size,
						 i_node		sender);

t_status	glue_message_initialize(void);

t_status	glue_message_clean(void);


/*
 * eop
 */

#endif
