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
 * updated       julien quintard   [wed nov 24 14:18:15 2010]
 */

#ifndef GLUE_SEGMENT_H
#define GLUE_SEGMENT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_segment()				\
  extern d_segment	glue_segment_dispatch

#define		machine_call_segment(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_segment_dispatch._function_ != NULL)			\
        _r_ = glue_segment_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_segment()

#define		machine_data_o_segment()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../segment.c
 */

/*
 * ../segment.c
 */

t_error			glue_segment_read(i_segment		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz);

t_error			glue_segment_write(i_segment		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz);

t_error			glue_segment_copy(i_segment		dst,
					  t_paddr		offsd,
					  i_segment		src,
					  t_paddr		offss,
					  t_psize		sz);

t_error			glue_segment_initialize(void);

t_error			glue_segment_clean(void);


/*
 * eop
 */

#endif
