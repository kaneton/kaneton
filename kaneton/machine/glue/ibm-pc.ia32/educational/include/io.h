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
 * updated       julien quintard   [wed nov 24 14:16:52 2010]
 */

#ifndef GLUE_IO_H
#define GLUE_IO_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_io()					\
  extern d_io		glue_io_dispatch

#define		machine_call_io(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_io_dispatch._function_ != NULL)				\
        _r_ = glue_io_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_io()

#define		machine_data_o_io()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../io.c
 */

/*
 * ../io.c
 */

t_error			glue_io_grant(i_port			id,
				      i_task			task,
				      t_uint8			width);

t_error			glue_io_deny(i_port			id,
				     i_task			task,
				     t_uint8			width);

t_error			glue_io_initialize(void);


/*
 * eop
 */

#endif
