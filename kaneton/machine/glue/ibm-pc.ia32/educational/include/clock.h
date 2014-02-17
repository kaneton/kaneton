
/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../ibm-pc.ia32/educational/include/clock.h
 *
 * created       julien quintard   [wed jun  6 16:29:41 2007]
 * updated       julien quintard   [sun dec 19 12:45:13 2010]
 */

#ifndef GLUE_CLOCK_H
#define GLUE_CLOCK_H			1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the clock dispatcher.
 */

#define		machine_include_clock()					\
  extern d_clock	glue_clock_dispatch

/*
 * this macro-function dispatches the clock calls.
 */

#define		machine_call_clock(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_clock_dispatch.clock_ ## _function_ != NULL)		\
        _r_ = glue_clock_dispatch.clock_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in the 'm_clock' type.
 */

#define		machine_data_m_clock()

/*
 * ---------- includes --------------------------------------------------------
 */

#include <core/clock.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../clock.c
 */

/*
 * ../clock.c
 */

t_status		glue_clock_update(t_uint32		millisecond);

t_status		glue_clock_current(s_clock*		clock);

t_status		glue_clock_initialize(void);

t_status		glue_clock_clean(void);


/*
 * eop
 */

#endif
