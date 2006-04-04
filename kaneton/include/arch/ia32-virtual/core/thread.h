/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/kaneton/include/core/thread.h
 *
 * created       renaud voltz   [tue apr  4 03:10:52 2006]
 * updated       renaud voltz   [tue apr  4 03:10:52 2006]
 */

#ifndef IA32_CORE_THREAD_H
#define IA32_CORE_THREAD_H       1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* #include <core/thread.h> */

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the architecture dependent data for the as object
 */

typedef struct
{
  /*                                                                [cut] k4   */

  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;
  t_uint32		esi;
  t_uint32		edi;
  t_uint32		eip;
  t_uint32		esp;
  t_uint32		ebp;
  t_uint16		cs;
  t_uint16		ds;
  t_uint32		eflags;

  /*                                                               [cut] /k4   */
}			__attribute__ ((packed)) ao_thread;


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

#define         machdep_data_m_thread()

#define         machdep_data_o_thread()					\
  ao_thread			machddep

#endif
