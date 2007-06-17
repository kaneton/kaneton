/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/include/sched.h
 *
 * created       julien quintard   [wed jun  6 16:24:14 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:55:54 2007]
 */

#ifndef GLUE_SCHED_H
#define GLUE_SCHED_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/id.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_sched()					\
  extern d_sched		sched_dispatch

#define		machine_call_sched(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (sched_dispatch._function_ != NULL)				\
        _r_ = sched_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_sched()					\
  struct								\
  {									\
    /*                                                   [cut] k3   */	\
    i_timer			timer;					\
									\
    i_thread			mmx_context;				\
    /*                                                   [cut] /k3   */	\
  }				machdep;

#define		machine_data_o_sched()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../sched.c
 */

/*
 * ../sched.c
 */

t_error			glue_sched_quantum(t_quantum		quantum);

t_error			glue_sched_yield(i_cpu			cpuid);

t_error			glue_sched_init(void);

t_error			glue_sched_clean(void);

void			glue_sched_switch_extended(i_event	id);

t_error			glue_sched_switch(i_thread		elected);


/*
 * eop
 */

#endif
