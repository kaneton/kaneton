/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...include/arch/ibm-pc.ia32-virtual/event.h
 *
 * created       julien quintard   [wed jun  6 16:15:26 2007]
 * updated       julien quintard   [thu jun  7 15:33:33 2007]
 */

#ifndef ARCHITECTURE_EVENT_H
#define ARCHITECTURE_EVENT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_event()					\
  extern d_event		event_dispatch

#define		machdep_call_event(_function_, _args_...)		\
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

#define		machdep_data_m_event()

#define		machdep_data_o_event()

#endif
