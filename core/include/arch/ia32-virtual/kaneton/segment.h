/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/arch/ia32-virtual/kaneton/segment.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       matthieu bucchianeri   [sun jan 15 18:18:41 2006]
 */

#ifndef IA32_KANETON_SEGMENT_H
#define IA32_KANETON_SEGMENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <kaneton/segment.h>*/

/*
 * ---------- macros ----------------------------------------------------------
 */

#define		machdep_include_segment()				\
  extern i_segment		segment_interface

#define		machdep_call_segment(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (segment_interface._function_ != NULL)				\
        _r_ = segment_interface._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_segment()

#define		machdep_data_o_segment()

#endif
