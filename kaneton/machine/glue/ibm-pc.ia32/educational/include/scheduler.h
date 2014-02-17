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
 * updated       julien quintard   [sun dec 19 16:18:38 2010]
 */

#ifndef GLUE_SCHEDULER_H
#define GLUE_SCHEDULER_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the scheduler dispatcher.
 */

#define		machine_include_scheduler()				\
  extern d_scheduler	glue_scheduler_dispatch

/*
 * this macro-function dispatches the scheduler calls.
 */

#define		machine_call_scheduler(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_scheduler_dispatch.scheduler_ ## _function_ != NULL)	\
        _r_ = glue_scheduler_dispatch.scheduler_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_scheduler' i.e the scheduler
 * manager.
 *
 * more precisely, the identifier of the timer used to trigger the
 * scheduler's context switche routine is kept. this identifier will,
 * for example, be used to adjust the delay between the scheduler's
 * context switches.
 */

#define		machine_data_m_scheduler()				\
  struct								\
  {									\
    i_timer			timer;					\
  }				machine;

/*
 * this macro-function includes data in 'o_scheduler'.
 */

#define		machine_data_o_scheduler()

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

t_status		glue_scheduler_dump(void);

t_status		glue_scheduler_start(i_cpu		cpu);

t_status		glue_scheduler_stop(i_cpu		cpu);

void			glue_scheduler_switch(void);

t_status		glue_scheduler_quantum(t_quantum	quantum);

t_status		glue_scheduler_yield(void);

t_status		glue_scheduler_initialize(void);


/*
 * eop
 */

#endif
