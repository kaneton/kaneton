/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../ibm-pc.ia32/educational/include/event.h
 *
 * created       julien quintard   [wed jun  6 16:15:26 2007]
 * updated       julien quintard   [sat feb  5 13:30:56 2011]
 */

#ifndef GLUE_EVENT_H
#define GLUE_EVENT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the event dispatcher.
 */

#define		machine_include_event()					\
  extern d_event	glue_event_dispatch

/*
 * this macro-function dispatches the CPU calls.
 */

#define		machine_call_event(_function_, _args_...)		\
  (									\
    {									\
      t_status	_r_ = STATUS_OK;						\
									\
      if (glue_event_dispatch.event_ ## _function_ != NULL)		\
        _r_ = glue_event_dispatch.event_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_event'.
 */

#define		machine_data_m_event()

/*
 * this macro-function includes data in 'o_event'.
 */

#define		machine_data_o_event()

/*
 * this macro-function includes data in 'o_event_message'.
 */

#define		machine_data_o_event_message()

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/event.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../event.c
 */

/*
 * ../event.c
 */


/*
 * eop
 */

#endif
