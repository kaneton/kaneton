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
 * updated       julien quintard   [fri dec 10 21:19:17 2010]
 */

#ifndef GLUE_MESSAGE_H
#define GLUE_MESSAGE_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_message()				\
  extern d_message	glue_message_dispatch

#define		machine_call_message(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_message_dispatch.message_ ## _function_ != NULL)		\
        _r_ = glue_message_dispatch.message_ ## _function_(_args_);	\
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

t_error			glue_message_yield(i_thread		thread,
					    t_error		code);

t_error			glue_message_return_info(i_thread	thread,
						 t_error	code,
						 t_vsize	size,
						 i_node		sender);

t_error			glue_message_initialize(void);

t_error			glue_message_clean(void);


/*
 * eop
 */

#endif
