/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/thread.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:32:54 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:32:54 2006]
 */

#ifndef OCTANE_CORE_THREAD_H
#define OCTANE_CORE_THREAD_H       1

/*
 * ---------- dependencies ----------------------------------------------------
 */

//#include <core/thread.h>

/*                                                                [cut] k4   */

/*
 *
 */

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
