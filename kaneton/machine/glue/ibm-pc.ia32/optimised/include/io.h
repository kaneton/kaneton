/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/include/io.h
 *
 * created       julien quintard   [wed jun  6 16:19:54 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:54:03 2007]
 */

#ifndef GLUE_IO_H
#define GLUE_IO_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_io()					\
  extern d_io		io_dispatch

#define		machine_call_io(_function_, _args_...)			\
  (									\
    {									\
      t_status _r_ = STATUS_OK;					\
									\
      if (io_dispatch._function_ != NULL)				\
        _r_ = io_dispatch._function_(_args_);				\
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

t_status		glue_io_grant(i_port			id,
				      i_task			task,
				      t_uint8			width);

t_status		glue_io_deny(i_port			id,
				     i_task			task,
				     t_uint8			width);

t_status		glue_io_initialize(void);


/*
 * eop
 */

#endif
