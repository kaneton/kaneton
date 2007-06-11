/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/segment.h
 *
 * created       enguerrand raymond   [wed oct 18 14:18:29 2006]
 * updated       enguerrand raymond   [wed oct 18 14:18:38 2006]
 */

#ifndef MIPS64_CORE_SEGMENT_H
#define MIPS64_CORE_SEGMENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <core/segment.h>*/


/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_segment()				\
  extern d_segment		segment_dispatch

#define		machine_call_segment(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (segment_dispatch._function_ != NULL)				\
        _r_ = segment_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_segment()

#define		machine_data_o_segment()

#endif
