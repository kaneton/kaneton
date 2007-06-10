/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...nclude/arch/ibm-pc.ia32-virtual/thread.h
 *
 * created       julien quintard   [wed jun  6 16:27:09 2007]
 * updated       julien quintard   [thu jun  7 15:35:34 2007]
 */

#ifndef ARCHITECTURE_THREAD_H
#define ARCHITECTURE_THREAD_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX temporary */
#include <libia32.h>

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
  struct								\
  {									\
    t_ia32_tss*		tss;						\
  }			machdep;

#define         machdep_data_o_thread()					\
  struct								\
  {									\
    /*                                                  [cut] k3   */	\
    t_ia32_context	context;					\
									\
    union								\
    {									\
      t_x87_state	x87;						\
      t_sse_state	sse;						\
    }			u;						\
    /*                                                  [cut] /k3   */	\
  }			machdep;

#endif
