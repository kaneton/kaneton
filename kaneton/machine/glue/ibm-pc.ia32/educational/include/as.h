/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...lue/ibm-pc.ia32/educational/include/as.h
 *
 * created       julien quintard   [sun jun  3 23:54:56 2007]
 * updated       julien quintard   [wed nov 24 14:14:44 2010]
 */

#ifndef GLUE_AS_H
#define GLUE_AS_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_as()					\
  extern d_as			glue_as_dispatch

#define		machine_call_as(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_as_dispatch._function_ != NULL)				\
        _r_ = glue_as_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_as()

#define		machine_data_o_as()					\
  struct								\
  {									\
    t_ia32_directory		pd;					\
  }				machine;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../as.c
 */

/*
 * ../as.c
 */

t_error			glue_as_reserve(i_task			tskid,
					i_as*			asid);


/*
 * eop
 */

#endif
