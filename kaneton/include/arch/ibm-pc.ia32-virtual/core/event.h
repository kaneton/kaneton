/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/include/arch/machdep/core/event.h
 *
 * created       julien quintard   [wed jun  6 16:15:26 2007]
 * updated       julien quintard   [wed jun  6 16:15:39 2007]
 */

#ifndef GUARD_IA32_CORE_EVENT
#define GUARD_IA32_CORE_EVENT		1

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
