/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...clude/arch/ibm-pc.ia32-virtual/segment.h
 *
 * created       julien quintard   [wed jun  6 16:25:05 2007]
 * updated       julien quintard   [thu jun  7 15:31:30 2007]
 */

#ifndef ARCHITECTURE_SEGMENT_H
#define ARCHITECTURE_SEGMENT_H		1

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

#endif
