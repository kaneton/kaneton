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
 * updated       julien quintard   [sat jan  8 13:58:34 2011]
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
      t_error   _r_ = ERROR_OK;						\
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
 * note that terms have been introduced in order to distinguish the
 * thread's stack i.e 'stack' with its kernel stack i.e 'pile'.
 *
 * the 'error' attribute stored the error code whenever an exception
 * occurs. XXX this should be placed somewhere else!
 */

#define         machine_data_o_thread()					\
  struct								\
  {									\
    t_vaddr		pile;						\
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

t_error			glue_thread_dump(void);

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
