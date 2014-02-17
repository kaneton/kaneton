/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...ibm-pc.ia32/educational/include/thread.h
 *
 * created       julien quintard   [wed jun  6 16:27:09 2007]
 * updated       matthieu bucchianeri   [wed jan  9 12:08:26 2008]
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
      t_status   _r_ = STATUS_OK;					\
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
									\
    t_uint16		core_cs;					\
    t_uint16		core_ds;					\
    t_uint16		driver_cs;					\
    t_uint16		driver_ds;					\
    t_uint16		service_cs;					\
    t_uint16		service_ds;					\
    t_uint16		program_cs;					\
    t_uint16		program_ds;					\
  }			machine;

#define         machine_data_o_thread()					\
  struct								\
  {									\
    t_vaddr		interrupt_stack;				\
									\
    union								\
    {									\
      t_x87_state	x87;						\
      t_sse_state	sse;						\
    }			u;						\
  }			machine;

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

t_status		glue_thread_clone(i_task		taskid,
					  i_thread		old,
					  i_thread*		new);

t_status		glue_thread_reserve(i_task		taskid,
					    i_thread*		threadid);

t_status		glue_thread_load(i_thread		threadid,
					 t_thread_context	context);

t_status		glue_thread_store(i_thread		threadid,
					  t_thread_context*	context);

t_status		glue_thread_args(i_thread		threadid,
					 const void*	       	args,
					 t_vsize		size);

t_status		glue_thread_initialize(void);


/*
 * eop
 */

#endif
