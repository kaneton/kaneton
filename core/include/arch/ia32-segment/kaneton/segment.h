/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/arch/ia32-segment/kaneton/segment.h
 *
 * created       julien quintard   [fri feb 11 02:23:41 2005]
 * updated       julien quintard   [fri mar 10 04:04:36 2006]
 */

#ifndef IA32_KANETON_SEGMENT_H
#define IA32_KANETON_SEGMENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <kaneton/segment.h>*/

/*                                                                [cut] k2   */

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

/*                                                               [cut] /k2   */

/*
 * ---------- macro functions -------------------------------------------------
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
