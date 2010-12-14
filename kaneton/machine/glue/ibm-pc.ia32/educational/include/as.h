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
 * updated       julien quintard   [mon dec 13 11:13:06 2010]
 */

#ifndef GLUE_AS_H
#define GLUE_AS_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define		machine_include_as()					\
  extern d_as			glue_as_dispatch

#define		machine_call_as(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_as_dispatch.as_ ## _function_ != NULL)			\
        _r_ = glue_as_dispatch.as_ ## _function_(_args_);		\
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
 * ---------- dependencies ----------------------------------------------------
 */

#include <modules/modules.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../as.c
 */

/*
 * ../as.c
 */

t_error			glue_as_show(i_as			id,
				     mt_margin			margin);

t_error			glue_as_reserve(i_task			tskid,
					i_as*			asid);


/*
 * eop
 */

#endif
