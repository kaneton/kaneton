/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/include/arch/machdep/core/sched.h
 *
 * created       julien quintard   [wed jun  6 16:24:14 2007]
 * updated       julien quintard   [wed jun  6 20:48:56 2007]
 */

#ifndef GUARD_IA32_CORE_SCHED
#define GUARD_IA32_CORE_SCHED		1

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
  struct								\
  {									\
    /*                                                   [cut] k3   */	\
    i_timer			timer;					\
									\
    i_thread			mmx_context;				\
    /*                                                   [cut] /k3   */	\
  }				machdep;

#define		machdep_data_o_sched()

#endif
