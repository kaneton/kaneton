
/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...machine/glue/ibm-pc.ia32/include/timer.h
 *
 * created       julien quintard   [wed jun  6 16:29:41 2007]
 * updated       julien quintard   [mon jun 11 05:54:33 2007]
 */

#ifndef GLUE_TIMER_H
#define GLUE_TIMER_H			1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_timer()					\
  extern d_timer		timer_dispatch

#define		machine_call_timer(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;					\
									\
      if (timer_dispatch._function_ != NULL)				\
        _r_ = timer_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_timer()

#define		machine_data_o_timer()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../timer.c
 */

/*
 * ../timer.c
 */

t_status		glue_timer_initialize(void);


/*
 * eop
 */

#endif
