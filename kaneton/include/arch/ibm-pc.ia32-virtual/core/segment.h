/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ibm-pc.ia32-virtual/core/segment.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       matthieu bucchianeri   [tue feb  6 23:20:19 2007]
 */

#ifndef IA32_CORE_SEGMENT_H
#define IA32_CORE_SEGMENT_H	1

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

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PMODE_GDT_CORE_CS	0x1
#define PMODE_GDT_CORE_DS	0x2
#define PMODE_GDT_DRIVER_CS	0x3
#define PMODE_GDT_DRIVER_DS	0x4
#define PMODE_GDT_SERVICE_CS	0x5
#define PMODE_GDT_SERVICE_DS	0x6
#define PMODE_GDT_PROGRAM_CS	0x7
#define PMODE_GDT_PROGRAM_DS	0x8

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/segment.h>

#endif
