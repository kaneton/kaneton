/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/io.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:31:11 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:31:12 2006]
 */

#ifndef OCTANE_CORE_IO_H
#define OCTANE_CORE_IO_H	1

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
