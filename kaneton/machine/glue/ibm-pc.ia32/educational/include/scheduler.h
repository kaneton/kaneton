/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...-pc.ia32/educational/include/scheduler.h
 *
 * created       julien quintard   [wed jun  6 16:24:14 2007]
 * updated       matthieu bucchianeri   [wed jan  9 12:07:11 2008]
 */

#ifndef GLUE_SCHEDULER_H
#define GLUE_SCHEDULER_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/id.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_scheduler()				\
  extern d_scheduler		scheduler_dispatch

#define		machine_call_scheduler(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (scheduler_dispatch._function_ != NULL)			\
        _r_ = scheduler_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_scheduler()				\
  struct								\
  {									\
    /* [cut k3] */							\
    i_timer			timer;					\
    /* [/cut] */							\
  }				machdep;

#define		machine_data_o_scheduler()				\
  struct								\
  {									\
    /* [cut k3] */							\
    i_thread			mmx_context;				\
    /* [/cut] */							\
  }				machdep;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../scheduler.c
 */

/*
 * ../scheduler.c
 */

t_error			glue_scheduler_quantum(t_quantum	quantum);

t_error			glue_scheduler_initialize(void);

t_error			glue_scheduler_clean(void);

void			glue_scheduler_switch_extended(i_event	id);

t_error			glue_scheduler_switch(i_thread		elected);


/*
 * eop
 */

#endif
