
/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../ibm-pc.ia32/educational/include/timer.h
 *
 * created       julien quintard   [wed jun  6 16:29:41 2007]
 * updated       julien quintard   [sun dec 19 16:23:00 2010]
 */

#ifndef GLUE_TIMER_H
#define GLUE_TIMER_H			1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the timer dispatcher.
 */

#define		machine_include_timer()					\
  extern d_timer	glue_timer_dispatch

/*
 * this macro-function dispatches the timer calls.
 */

#define		machine_call_timer(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_timer_dispatch.timer_ ## _function_ != NULL)		\
        _r_ = glue_timer_dispatch.timer_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_timer'.
 */

#define		machine_data_m_timer()

/*
 * this macro-function includes data in 'o_timer'.
 */

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

t_status		glue_timer_clean(void);


/*
 * eop
 */

#endif
