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
 * updated       julien quintard   [sat feb  5 15:40:42 2011]
 */

#ifndef GLUE_AS_H
#define GLUE_AS_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the address space dispatcher.
 */

#define		machine_include_as()					\
  extern d_as			glue_as_dispatch

/*
 * this macro-function dispatches the address space calls.
 */

#define		machine_call_as(_function_, _args_...)			\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_as_dispatch.as_ ## _function_ != NULL)			\
        _r_ = glue_as_dispatch.as_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in the 'm_as' type.
 *
 * the 'pd' attribute represents the architecture's kernel PD - Page
 * Directory.
 */

#define		machine_data_m_as()					\
  struct								\
  {									\
    at_pd		pd;						\
  }			machine;

/*
 * this macro-function includes data in the address space object 'o_as':
 * the address space IA32-specific PD - Page directory's location.
 */

#define		machine_data_o_as()					\
  struct								\
  {									\
    t_paddr		pd;						\
  }			machine;

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

t_status		glue_as_show(i_as			id,
				     mt_margin			margin);

t_status		glue_as_dump(void);

t_status		glue_as_reserve(i_task			task,
					i_as*			as);


/*
 * eop
 */

#endif
