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
 * updated       julien quintard   [mon dec 13 11:13:35 2010]
 */

#ifndef GLUE_IO_H
#define GLUE_IO_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define		machine_include_io()					\
  extern d_io		glue_io_dispatch

#define		machine_call_io(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_io_dispatch.io_ ## _function_ != NULL)			\
        _r_ = glue_io_dispatch.io_ ## _function_(_args_);		\
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

t_error			glue_io_grant(i_task			task,
				      i_port			port,
				      t_uint8			width);

t_error			glue_io_deny(i_task			task,
				     i_port			port,
				     t_uint8			width);

t_error			glue_io_read_8(i_task			task,
				       i_port			port,
				       t_uint8*			data);

t_error			glue_io_read_16(i_task			task,
					i_port			port,
					t_uint16*		data);

t_error			glue_io_read_32(i_task			task,
					i_port			port,
					t_uint32*		data);

t_error			glue_io_read_64(i_task			task,
					i_port			port,
					t_uint64*		data);

t_error			glue_io_write_8(i_task			task,
					i_port			port,
					t_uint8			data);

t_error			glue_io_write_16(i_task			task,
					 i_port			port,
					 t_uint16		data);

t_error			glue_io_write_32(i_task			task,
					 i_port			port,
					 t_uint32		data);

t_error			glue_io_write_64(i_task			task,
					 i_port			port,
					 t_uint64		data);

t_error			glue_io_initialize(void);


/*
 * eop
 */

#endif
