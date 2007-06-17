/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/include/event.h
 *
 * created       julien quintard   [wed jun  6 16:15:26 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:44:35 2007]
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
