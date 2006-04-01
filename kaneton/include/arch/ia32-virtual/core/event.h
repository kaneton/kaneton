/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/event.h
 *
 * created       renaud voltz   [mon feb 13 00:56:13 2006]
 * updated       julien quintard   [sat apr  1 22:57:19 2006]
 */

#ifndef IA32_CORE_EVENT_H
#define IA32_CORE_EVENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* #include <core/event.h> */


/*
 * generic event handler type.
 */

typedef void                    (*t_event_handler)(t_uint32);

/*
 * XXX EVENT move me ! (should be in /core/include/core/event.h)
 */

typedef enum
  {
    EVENT_FUNCTION,
    EVENT_MESSAGE
  } e_event_type;

/*
 * XXX EVENT move me ! (should be in /core/include/core/event.h)
 */

typedef union
{
  t_event_handler               function;
  t_tskid                       taskid;
}                               u_event_handler;

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_event()					\
  extern i_event		event_interface

#define		machdep_call_event(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (event_interface._function_ != NULL)				\
        _r_ = event_interface._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_event()

#define		machdep_data_o_event()

#endif
