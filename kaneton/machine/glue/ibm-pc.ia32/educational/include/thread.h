/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ibm-pc.ia32/educational/include/thread.h
 *
 * created       julien quintard   [wed jun  6 16:27:09 2007]
 * updated       julien quintard   [fri dec 17 14:47:11 2010]
 */

#ifndef GLUE_THREAD_H
#define GLUE_THREAD_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define         machine_include_thread()				\
  extern d_thread	glue_thread_dispatch

#define         machine_call_thread(_function_, _args_...)		\
  (									\
    {									\
      t_error   _r_ = ERROR_OK;						\
									\
      if (glue_thread_dispatch.thread_ ## _function_ != NULL)		\
        _r_ = glue_thread_dispatch.thread_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

#define         machine_data_m_thread()					\
  struct								\
  {									\
    t_ia32_tss*		tss;						\
									\
    t_uint16		kernel_cs;					\
    t_uint16		kernel_ds;					\
    t_uint16		driver_cs;					\
    t_uint16		driver_ds;					\
    t_uint16		service_cs;					\
    t_uint16		service_ds;					\
    t_uint16		guest_cs;					\
    t_uint16		guest_ds;					\
  }			machine;

#define         machine_data_o_thread()					\
  struct								\
  {									\
    t_vaddr		interrupt_stack;				\
									\
    t_uint32		error;						\
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

t_error			glue_thread_show(i_thread		id,
					 mt_margin		margin);

t_error			glue_thread_reserve(i_task		taskid,
					    i_thread*		threadid);

t_error			glue_thread_load(i_thread		threadid,
					 s_thread_context	context);

t_error			glue_thread_store(i_thread		threadid,
					  s_thread_context*	context);

t_error			glue_thread_args(i_thread		threadid,
					 const void*	       	args,
					 t_vsize		size);

t_error			glue_thread_initialize(void);


/*
 * eop
 */

#endif
