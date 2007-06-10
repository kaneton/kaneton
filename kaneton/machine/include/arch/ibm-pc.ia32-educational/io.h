/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/include/arch/ibm-pc.ia32-virtual/io.h
 *
 * created       julien quintard   [wed jun  6 16:19:54 2007]
 * updated       julien quintard   [thu jun  7 15:34:20 2007]
 */

#ifndef ARCHITECTURE_IO_H
#define ARCHITECTURE_IO_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_io()					\
  extern d_io		io_dispatch

#define		machdep_call_io(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (io_dispatch._function_ != NULL)				\
        _r_ = io_dispatch._function_(_args_);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_io()

#define		machdep_data_o_io()

#endif
