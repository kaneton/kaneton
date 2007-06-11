/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/io.h
 *
 * created       enguerrand raymond   [wed oct 18 14:17:07 2006]
 * updated       enguerrand raymond   [wed oct 18 14:20:28 2006]
 */

#ifndef MIPS64_CORE_IO_H
#define MIPS64_CORE_IO_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <core/io.h>*/

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_io()					\
  extern d_io		io_dispatch

#define		machine_call_io(_function_, _args_...)			\
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

#define		machine_data_m_io()

#define		machine_data_o_io()

#endif
