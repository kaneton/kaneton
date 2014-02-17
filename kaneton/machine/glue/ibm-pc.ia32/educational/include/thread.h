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
 * updated       julien quintard   [sun jan 30 12:32:44 2011]
 */

#ifndef GLUE_THREAD_H
#define GLUE_THREAD_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the thread dispatcher.
 */

#define         machine_include_thread()				\
  extern d_thread	glue_thread_dispatch

/*
 * this macro-function dispatches the thread calls.
 */

#define         machine_call_thread(_function_, _args_...)		\
  (									\
    {									\
      t_status   _r_ = STATUS_OK;						\
									\
      if (glue_thread_dispatch.thread_ ## _function_ != NULL)		\
        _r_ = glue_thread_dispatch.thread_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in the thread manager's structure
 * 'm_thread'.
 *
 * the embedded data include the TSS - Task State Segment structure's
 * location along with the several segment selectors CS - Code Segment
 * and DS - Data Segment for the various task classes: kernel, driver,
 * service and guest.
 */

#define         machine_data_m_thread()					\
  struct								\
  {									\
    t_vaddr		tss;						\
									\
    struct								\
    {									\
      struct								\
      {									\
        t_uint16	cs;						\
        t_uint16	ds;						\
      }			kernel;						\
									\
      struct								\
      {									\
        t_uint16	cs;						\
        t_uint16	ds;						\
      }			driver;						\
									\
      struct								\
      {									\
        t_uint16	cs;						\
        t_uint16	ds;						\
      }			service;					\
									\
      struct								\
      {									\
        t_uint16	cs;						\
        t_uint16	ds;						\
      }			guest;						\
    }			selectors;					\
  }			machine;

/*
 * this macro-function includes data in the thread object's structure
 * 'o_thread'.
 *
 * these include the thread's kernel stack referred to as the _pile_.
 * indeed, whenever an interrupt or exception is triggered, the
 * thread's context is stored on the thread's pile i.e not its stack.
 *
 * however, a special case exists for ring0 threads i.e kernel threads.
 * when these threads are interrupted, their context is stored on the
 * current stack since no change in privilege occurs.
 *
 * the 'context' attribute contains the location, within the thread's
 * address space, of the thread's IA32 context. this context, depending on
 * the thread's privilege, can be located either on the stack or the pile.
 *
 * note that the 'stack.pointer' and 'pile.pointer' are static values---i.e
 * do not change over time---referencing the top of the stack/pile.
 */

#define         machine_data_o_thread()					\
  struct								\
  {									\
    t_vaddr		context;					\
									\
    struct								\
    {									\
      t_vaddr		pointer;					\
    }			stack;						\
									\
    struct								\
    {									\
      t_vaddr		base;						\
      t_vsize		size;						\
      t_vaddr		pointer;					\
    }			pile;						\
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

t_status		glue_thread_show(i_thread		id,
					 mt_margin		margin);

t_status		glue_thread_dump(void);

t_status		glue_thread_reserve(i_task		task,
					    t_priority		priority,
					    t_vaddr		stack,
					    t_vsize		stacksz,
					    t_vaddr		entry,
					    i_thread*		id);

t_status		glue_thread_release(i_thread		id);

t_status		glue_thread_load(i_thread		id,
					 s_thread_context	context);

t_status		glue_thread_store(i_thread		id,
					  s_thread_context*	context);

t_status		glue_thread_arguments(i_thread		id,
					      void*		arguments,
					      t_vsize		size);

t_status		glue_thread_initialize(void);


/*
 * eop
 */

#endif
