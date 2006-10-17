/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/segment.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:32:19 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:32:19 2006]
 */

#ifndef OCTANE_CORE_SEGMENT_H
#define OCTANE_CORE_SEGMENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <core/segment.h>*/


/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_segment()				\
  extern d_segment		segment_dispatch

#define		machdep_call_segment(_function_, _args_...)		\
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

#define		machdep_data_m_segment()

#define		machdep_data_o_segment()

#endif
