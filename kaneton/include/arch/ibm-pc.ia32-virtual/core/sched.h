/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ibm-pc.ia32-virtual/core/sched.h
 *
 * created       matthieu bucchianeri   [sat jun  3 22:47:21 2006]
 * updated       matthieu bucchianeri   [tue feb  6 23:19:56 2007]
 */

#ifndef IA32_CORE_SCHED_H
#define IA32_CORE_SCHED_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_sched()					\
  extern d_sched		sched_dispatch

#define		machdep_call_sched(_function_, _args_...)		\
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

#define		machdep_data_m_sched()					\
  am_sched			machdep

#define		machdep_data_o_sched()

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the architecture dependent data for the sched manager.
 */

typedef struct
{
/*                                                                [cut] k3   */

  i_timer			timer;

  i_thread			mmx_context;

/*                                                               [cut] /k3   */
}				am_sched;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/sched.h>

#endif
