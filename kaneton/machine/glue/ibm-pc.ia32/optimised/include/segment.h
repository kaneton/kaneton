/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/include/segment.h
 *
 * created       julien quintard   [wed jun  6 16:25:05 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:58:04 2007]
 */

#ifndef GLUE_SEGMENT_H
#define GLUE_SEGMENT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_segment()				\
  extern d_segment		segment_dispatch

#define		machine_call_segment(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;					\
									\
      if (segment_dispatch._function_ != NULL)				\
        _r_ = segment_dispatch._function_(_args_);			\
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

t_status		glue_segment_clean(void);


/*
 * eop
 */

#endif
