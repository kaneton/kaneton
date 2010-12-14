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
 * updated       julien quintard   [mon dec 13 11:14:14 2010]
 */

#ifndef GLUE_EVENT_H
#define GLUE_EVENT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define		machine_include_event()					\
  extern d_event	glue_event_dispatch

#define		machine_call_event(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_event_dispatch.event_ ## _function_ != NULL)		\
        _r_ = glue_event_dispatch.event_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_event()

#define		machine_data_o_event()

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

t_error			glue_event_enable(void);

t_error			glue_event_disable(void);

t_error			glue_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler,
					   t_vaddr		data);

t_error			glue_event_release(i_event		id);

void			pf_handler(t_id				id,
				   t_vaddr			data,
				   t_uint32			error_code);

t_error			glue_event_initialize(void);

t_error			glue_event_clean(void);


/*
 * eop
 */

#endif
