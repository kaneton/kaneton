/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/sched.h
 *
 * created       enguerrand raymond   [wed oct 18 14:18:01 2006]
 * updated       enguerrand raymond   [wed oct 18 14:18:15 2006]
 */

#ifndef MIPS64_CORE_SCHED_H
#define MIPS64_CORE_SCHED_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <core/sched.h>*/

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the architecture dependent data for the sched manager.
 */

typedef struct
{
  i_timer			timer;

  i_thread			mmx_context;
}				am_sched;

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

#endif
