/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...achine/glue/ibm-pc.ia32/include/thread.h
 *
 * created       julien quintard   [wed jun  6 16:27:09 2007]
 * updated       matthieu bucchianeri   [sun jun 17 16:18:31 2007]
 */

#ifndef GLUE_THREAD_H
#define GLUE_THREAD_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define         machine_include_thread()				\
  extern d_thread                thread_dispatch

#define         machine_call_thread(_function_, _args_...)		\
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

#define         machine_data_m_thread()					\
  struct								\
  {									\
    t_ia32_tss*		tss;						\
  }			machdep;

#define         machine_data_o_thread()					\
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

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/thread.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../thread.c
 */

/*
 * ../thread.c
 */

t_error			glue_thread_clone(i_task		taskid,
					  i_thread		old,
					  i_thread*		new);

t_error			glue_thread_reserve(i_task		taskid,
					    i_thread*		threadid);

t_error			glue_thread_load(i_thread		threadid,
					 t_thread_context	context);

t_error			glue_thread_store(i_thread		threadid,
					  t_thread_context*	context);

t_error			glue_thread_initialize(void);


/*
 * eop
 */

#endif