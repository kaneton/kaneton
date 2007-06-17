/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...machine/glue/ibm-pc.ia32/include/event.h
 *
 * created       julien quintard   [wed jun  6 16:15:26 2007]
 * updated       matthieu bucchianeri   [sun jun 17 19:26:06 2007]
 */

#ifndef GLUE_EVENT_H
#define GLUE_EVENT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_event()					\
  extern d_event		event_dispatch

#define		machine_call_event(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (event_dispatch._function_ != NULL)				\
        _r_ = event_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_event()

#define		machine_data_o_event()

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

t_error			glue_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler);

t_error			glue_event_release(i_event		id);

t_error			glue_event_init(void);

t_error			glue_event_clean(void);


/*
 * eop
 */

#endif
