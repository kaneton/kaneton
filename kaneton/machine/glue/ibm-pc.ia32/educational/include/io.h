/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...lue/ibm-pc.ia32/educational/include/io.h
 *
 * created       julien quintard   [wed jun  6 16:19:54 2007]
 * updated       julien quintard   [sun dec 19 13:41:25 2010]
 */

#ifndef GLUE_IO_H
#define GLUE_IO_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the I/O dispatcher.
 */

#define		machine_include_io()					\
  extern d_io		glue_io_dispatch

/*
 * this macro-function dispatches the I/O calls.
 */

#define		machine_call_io(_function_, _args_...)			\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_io_dispatch.io_ ## _function_ != NULL)			\
        _r_ = glue_io_dispatch.io_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_io'.
 */

#define		machine_data_m_io()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../io.c
 */

/*
 * ../io.c
 */

t_status		glue_io_grant(i_task			task,
				      i_port			port,
				      t_width			width);

t_status		glue_io_deny(i_task			task,
				     i_port			port,
				     t_width			width);

t_status		glue_io_read(i_task			task,
				     i_port			port,
				     t_width			width,
				     void*			data);

t_status		glue_io_write(i_task			task,
				      i_port			port,
				      t_width			width,
				      t_uint64			data);

t_status		glue_io_initialize(void);


/*
 * eop
 */

#endif
