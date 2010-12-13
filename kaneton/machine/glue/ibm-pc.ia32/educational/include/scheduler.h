/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...-pc.ia32/educational/include/scheduler.h
 *
 * created       julien quintard   [wed jun  6 16:24:14 2007]
 * updated       julien quintard   [sat dec 11 10:27:03 2010]
 */

#ifndef GLUE_SCHEDULER_H
#define GLUE_SCHEDULER_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_scheduler()				\
  extern d_scheduler	glue_scheduler_dispatch

#define		machine_call_scheduler(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_scheduler_dispatch.scheduler_ ## _function_ != NULL)	\
        _r_ = glue_scheduler_dispatch.scheduler_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_scheduler()				\
  struct								\
  {									\
    i_timer			timer;					\
  }				machine;

#define		machine_data_o_scheduler()				\
  struct								\
  {									\
    i_thread			mmx_context;				\
  }				machine;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/id.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../scheduler.c
 */

/*
 * ../scheduler.c
 */

t_error			glue_scheduler_dump(void);

t_error			glue_scheduler_stop(i_cpu		cpu);

void			glue_scheduler_switch_handler(void);

t_error			glue_scheduler_quantum(t_quantum	quantum);

t_error			glue_scheduler_yield(void);

t_error			glue_scheduler_initialize(void);

t_error			glue_scheduler_clean(void);

void			glue_scheduler_switch_extended(i_event	id);


/*
 * eop
 */

#endif
