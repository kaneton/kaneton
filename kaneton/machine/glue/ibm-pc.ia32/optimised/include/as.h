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
 * updated       julien quintard   [fri may  1 18:01:07 2009]
 */

#ifndef GLUE_AS_H
#define GLUE_AS_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_as()					\
  extern d_as			as_dispatch

#define		machine_call_as(_function_, _args_...)			\
  (									\
    {									\
      t_status _r_ = STATUS_OK;					\
									\
      if (as_dispatch._function_ != NULL)				\
        _r_ = as_dispatch._function_(_args_);				\
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

t_status		glue_as_reserve(i_task			tskid,
					i_as*			asid);


/*
 * eop
 */

#endif
