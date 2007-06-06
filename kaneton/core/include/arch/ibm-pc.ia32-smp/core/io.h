/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/io.h
 *
 * created       matthieu bucchianeri   [sat jul 29 18:10:14 2006]
 * updated       matthieu bucchianeri   [sat jul 29 18:10:25 2006]
 */

#ifndef IA32_CORE_IO_H
#define IA32_CORE_IO_H	1

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
