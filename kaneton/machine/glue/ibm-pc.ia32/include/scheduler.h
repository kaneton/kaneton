/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...ine/glue/ibm-pc.ia32/include/scheduler.h
 *
 * created       julien quintard   [wed jun  6 16:24:14 2007]
 * updated       matthieu bucchianeri   [sun jun 17 21:52:20 2007]
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
    /*                                                   [cut] k3   */	\
    i_timer			timer;					\
									\
    i_thread			mmx_context;				\
    /*                                                   [cut] /k3   */	\
  }				machdep;

#define		machine_data_o_scheduler()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../scheduler.c
 */

/*
 * ../scheduler.c
 */

t_error			glue_scheduler_quantum(t_quantum	quantum);

t_error			glue_scheduler_yield(i_cpu			cpuid);

t_error			glue_scheduler_init(void);

t_error			glue_scheduler_clean(void);

void			glue_scheduler_switch_extended(i_event	id);

t_error			glue_scheduler_switch(i_thread		elected);


/*
 * eop
 */

#endif
