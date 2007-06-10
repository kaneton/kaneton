/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/thread.h
 *
 * created       enguerrand raymond   [wed oct 18 14:19:28 2006]
 * updated       enguerrand raymond   [wed oct 18 14:19:48 2006]
 */

#ifndef MIPS64_CORE_THREAD_H
#define MIPS64_CORE_THREAD_H       1

/*
 * ---------- dependencies ----------------------------------------------------
 */

//#include <core/thread.h>

typedef struct
{

}			am_thread;

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{

}			ao_thread;

/*
 * ---------- macro functions -------------------------------------------------
 */

#define         machdep_include_thread()				\
  extern d_thread                thread_dispatch

#define         machdep_call_thread(_function_, _args_...)		\
  (									\
    {									\
      t_error   _r_ = ERROR_NONE;					\
									\
      if (thread_dispatch._function_ != NULL)				\
        _r_ = thread_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define         machdep_data_m_thread()					\
  am_thread		machdep

#define         machdep_data_o_thread()					\
  ao_thread		machdep

/*                                                               [cut] /k4   */

#endif
