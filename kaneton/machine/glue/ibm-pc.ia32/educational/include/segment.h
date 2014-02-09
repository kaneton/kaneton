/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...bm-pc.ia32/educational/include/segment.h
 *
 * created       julien quintard   [wed jun  6 16:25:05 2007]
 * updated       julien quintard   [sun jan 16 13:41:34 2011]
 */

#ifndef GLUE_SEGMENT_H
#define GLUE_SEGMENT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the segment dispatcher.
 */

#define		machine_include_segment()				\
  extern d_segment	glue_segment_dispatch

/*
 * this macro-function dispatches the segment calls.
 */

#define		machine_call_segment(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_segment_dispatch.segment_ ## _function_ != NULL)		\
        _r_ = glue_segment_dispatch.segment_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_segment'.
 */

#define		machine_data_m_segment()				\
  struct								\
  {									\
    as_gdt	gdt;							\
  }		machine;

/*
 * this macro-function includes data in 'o_segment'.
 */

#define		machine_data_o_segment()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../segment.c
 */

/*
 * ../segment.c
 */

t_status		glue_segment_dump(void);

t_status		glue_segment_read(i_segment		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz);

t_status		glue_segment_write(i_segment		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz);

t_status		glue_segment_copy(i_segment		dst,
					  t_paddr		offsd,
					  i_segment		src,
					  t_paddr		offss,
					  t_psize		sz);

t_status		glue_segment_initialize(void);


/*
 * eop
 */

#endif
